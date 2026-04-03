#include "DataView.h"

#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Arrays/StructArray.h"
#include "SatisfactorySave/GameTypes/Properties/ArrayProperty.h"
#include "SatisfactorySave/GameTypes/Properties/StructProperty.h"
#include "SatisfactorySave/GameTypes/Structs/PropertyStruct.h"
#include "SatisfactorySave/GameTypes/Structs/VectorStruct.h"
#include "SatisfactorySave/Utils/StringUtils.h"
#include "SplineData.h"
#include "Utils/FilesystemUtil.h"
#include "Utils/GLMUtil.h"

namespace {
    namespace m = Satisfactory3DMap;

    void countAndSortObjects(m::DataView::SaveNode& node) {
        node.numObjects = 0;
        node.numActors = 0;
        for (auto& child : node.childNodes) {
            countAndSortObjects(child.second);
            node.numObjects += child.second.numObjects;
            node.numActors += child.second.numActors;
        }
        if (!node.objects.empty()) {
            for (const auto& obj : node.objects) {
                if (obj->isActor()) {
                    node.numActors++;
                } else {
                    node.numObjects++;
                }
            }
            std::stable_sort(node.objects.begin(), node.objects.end(), [](const auto& a, const auto& b) {
                return s::natLessCaseInsensitive(a->pathName(), b->pathName());
            });
        }
    }

    struct SplineMeshInstanceGpu {
        int32_t listOffset;
        int32_t offset0;
        int32_t offset1;
        int32_t _padding1_;
        float t0;
        float t1;
        float _padding2_[2];
        glm::vec4 forward;
    };
    static_assert(sizeof(SplineMeshInstanceGpu) == 4 * sizeof(int32_t) + 8 * sizeof(float),
        "SplineMeshInstanceGpu: Alignment issue!");

    glm::vec3 deCasteljau(const m::SplineSegmentGpu& segment, float t) {
        // Bezier control points
        const glm::vec3 b0 = glm::vec3(segment.p0);
        const glm::vec3 b3 = glm::vec3(segment.p1);
        // Unreal Engine seems to use cubic Hermite splines, convert to Bezier control points.
        const glm::vec3 b1 = b0 + glm::vec3(segment.tangent0) / 3.0f;
        const glm::vec3 b2 = b3 - glm::vec3(segment.tangent1) / 3.0f;

        // De Casteljau
        const glm::vec3 b01 = glm::mix(b0, b1, t);
        const glm::vec3 b11 = glm::mix(b1, b2, t);
        const glm::vec3 b21 = glm::mix(b2, b3, t);

        const glm::vec3 b02 = glm::mix(b01, b11, t);
        const glm::vec3 b12 = glm::mix(b11, b21, t);

        return glm::mix(b02, b12, t);
    }

    glm::vec3 determineInstanceForward(float t0, float t1,
        const std::vector<m::SplineSegmentGpu>& splineSegments, int32_t offset0, int32_t offset1) {
        // Determine spline segments
        int segmentIdx0 = offset0;
        int segmentIdx1 = offset0;
        while (t0 > splineSegments[segmentIdx0].len && segmentIdx0 < offset1 - 1) {
            t0 -= splineSegments[segmentIdx0].len;
            t1 -= splineSegments[segmentIdx1].len;
            segmentIdx0++;
            segmentIdx1++;
        }
        while (t1 > splineSegments[segmentIdx1].len && segmentIdx1 < offset1 - 1) {
            t1 -= splineSegments[segmentIdx1].len;
            segmentIdx1++;
        }
        const m::SplineSegmentGpu& segment0 = splineSegments[segmentIdx0];
        const m::SplineSegmentGpu& segment1 = splineSegments[segmentIdx1];

        const float segmentT0 = std::clamp(t0 / segment0.len, 0.0f, 1.0f);
        const float segmentT1 = std::clamp(t1 / segment1.len, 0.0f, 1.0f);

        glm::vec3 p0 = deCasteljau(segment0, segmentT0);
        glm::vec3 p1 = deCasteljau(segment1, segmentT1);

        // Use vector from start to end of a single instance as approximation of forward vector.
        const glm::vec3 forward = glm::normalize(p1 - p0);
        return forward;
    }

    struct PowerLineInstanceGpu {
        int32_t listOffset;
        int32_t _padding1_[3];
        glm::vec4 p0;
        glm::vec4 p1;
    };
    static_assert(sizeof(PowerLineInstanceGpu) == 4 * sizeof(int32_t) + 8 * sizeof(float),
        "PowerLineInstanceGpu: Alignment issue!");

    struct PowerLinePosition {
        glm::vec3 p0;
        glm::vec3 p1;
    };

    std::vector<PowerLinePosition> getPowerLinePositions(const s::SaveObject& obj) {
        auto& sa = obj.Object->Properties.get<s::ArrayProperty>("mWireInstances").get<s::StructArray>();
        if (sa.StructName() != "WireInstance") {
            throw std::runtime_error("Expect struct WireInstance!");
        }
        std::vector<PowerLinePosition> result;
        for (const auto& s : sa.Values) {
            const auto& ps = dynamic_cast<s::PropertyStruct&>(*s);
            const auto p0 = ps.Data.get<s::StructProperty>("Locations", 0).get<s::VectorStruct>().Data;
            const auto p1 = ps.Data.get<s::StructProperty>("Locations", 1).get<s::VectorStruct>().Data;
            result.emplace_back(m::glmCast(p0), m::glmCast(p1));
        }
        return result;
    }
} // namespace

Satisfactory3DMap::DataView::DataView(std::shared_ptr<Configuration> config)
    : config_(std::move(config)),
      selectedObject_(nullptr) {

    usePakSetting_ = BoolSetting::create("UsePakFiles", false);
    gameDirSetting_ = PathSetting::create("GameDirectory", PathSetting::PathType::Directory);
    config_->registerSetting(usePakSetting_);
    config_->registerSetting(gameDirSetting_);

    if (gameDirSetting_->getVal().empty()) {
        const auto& gameDirs = findGameDirs();
        if (!gameDirs.empty()) {
            gameDirSetting_->setVal(gameDirs[0]);
        }
    }
    if (usePakSetting_->getVal()) {
        if (!gameDirSetting_->getVal().empty()) {
            try {
                pakManager_ = s::PakManager::create(gameDirSetting_->getVal());
            } catch (const std::exception& ex) {
                spdlog::error("Error init PakManager: {}", ex.what());
                showErrors_.push_back(std::string("Error reading game dir: ") + ex.what());
            }
        } else {
            spdlog::warn("No game dir set!");
            showErrors_.emplace_back("No game dir found! Please go to File > Settings and select a game dir.");
        }
    }

    manager_ = std::make_unique<ModelManager>(pakManager_);
}

void Satisfactory3DMap::DataView::openSave(const std::filesystem::path& file) {
    if (file.empty()) {
        return;
    }
    if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file)) {
        spdlog::error("No regular file given!");
        return;
    }

    // cleanup
    proxy_list_.clear();
    object_proxy_map_.clear();
    lightweight_buildable_list_.clear();
    level_root_nodes_.clear();
    persistent_and_runtime_root_node_ = SaveNode();
    lightweight_buildable_root_node_ = SaveNode();
    all_root_node_ = SaveNode();
    lightweight_buildable_subsystem_ = nullptr;
    selectedObject_ = nullptr;

    // Delete first to reduce memory footprint.
    savegame_.reset();

    try {
        savegame_ = std::make_unique<s::SaveGame>(file);
        spdlog::info("Savegame header:\n{}", savegame_->mSaveHeader.toString());

        // Generate proxy wrappers
        const std::size_t numObjects = savegame_->allSaveObjects().size();
        proxy_list_.reserve(numObjects);
        object_proxy_map_.reserve(numObjects);
        for (int32_t i = 0; i < numObjects; i++) {
            const auto& obj = savegame_->allSaveObjects()[i];
            auto p = std::make_shared<ObjectProxy>(i, obj);
            proxy_list_.emplace_back(p);
            object_proxy_map_.emplace(obj, p);
        }

        // Lightweight buildable objects
        const char* lightweightBuildableSubsystemClassName = "/Script/FactoryGame.FGLightweightBuildableSubsystem";
        if (savegame_->isObjectClass(lightweightBuildableSubsystemClassName)) {
            const auto lbs = savegame_->getObjectsByClass(lightweightBuildableSubsystemClassName);
            if (lbs.size() != 1) {
                throw std::runtime_error("Expected only one object of type LightweightBuildableSubsystem!");
            }
            lightweight_buildable_subsystem_ =
                std::dynamic_pointer_cast<s::AFGLightweightBuildableSubsystem>(lbs[0]->Object);
        }
        if (lightweight_buildable_subsystem_ != nullptr) {
            int32_t next_id = static_cast<int32_t>(proxy_list_.size());
            auto& instanceMap = lightweight_buildable_subsystem_->mBuildableClassToInstanceArray;
            for (std::size_t i = 0; i < instanceMap.Keys.size(); i++) {
                const auto& className = instanceMap.Keys[i];
                auto& instances = instanceMap.Values[i];
                for (std::size_t j = 0; j < instances.size(); j++) {
                    auto& instance = instances[j];

                    // Game stores deleted objects until next load.
                    if (instance.BuiltWithRecipe.PathName.empty()) {
                        continue;
                    }

                    auto p = std::make_shared<ObjectProxy>(next_id, className.PathName, j, &instance);
                    next_id++;
                    proxy_list_.emplace_back(p);
                    lightweight_buildable_list_.emplace_back(p);
                }
            }
        }

        // Generate node structures
        level_root_nodes_.resize(savegame_->mPerLevelDataMap.size());
        for (std::size_t i = 0; i < savegame_->mPerLevelDataMap.size(); i++) {
            for (const auto& obj : savegame_->mPerLevelDataMap.Values[i].SaveObjects) {
                addToNode(object_proxy_map_.at(obj), level_root_nodes_[i]);
            }
        }
        for (const auto& obj : savegame_->mPersistentAndRuntimeData.SaveObjects) {
            addToNode(object_proxy_map_.at(obj), persistent_and_runtime_root_node_);
        }
        for (const auto& obj : lightweight_buildable_list_) {
            addToNode(obj, lightweight_buildable_root_node_);
        }

        // Count number of child objects in tree
        countAndSortObjects(all_root_node_);
        countAndSortObjects(persistent_and_runtime_root_node_);
        for (auto& node : level_root_nodes_) {
            countAndSortObjects(node);
        }

        // Model data and GPU structures
        std::vector<int32_t> actorIds;
        std::vector<glm::mat4> actorTransformations;

        pakModelDataList_.clear();
        std::vector<std::vector<int32_t>> pakActorListOffsets;

        modelDataList_.clear();
        modelDataList_.resize(manager_->models().size());
        std::vector<std::vector<int32_t>> actorListOffsets(manager_->models().size());

        splineModelDataList_.clear();
        splineModelDataList_.resize(manager_->splineModels().size());
        std::vector<std::vector<SplineSegmentGpu>> splineSegments(manager_->splineModels().size());
        std::vector<std::vector<SplineMeshInstanceGpu>> splineInstances(manager_->splineModels().size());
        powerLineData_.numInstances = 0;
        powerLineData_.instanceData.reset();
        std::vector<PowerLineInstanceGpu> powerLineInstances;

        for (const auto& proxy : proxy_list_) {
            if (proxy->isActor()) {
                const int32_t actorListOffset = static_cast<int32_t>(actorIds.size());
                actorIds.push_back(proxy->id());
                actorTransformations.push_back(proxy->getTransformMat());
                actorBufferPositions_.emplace(proxy->id(), actorListOffset);

                const auto& [modelType, idx] = manager_->classifyActor(proxy->className());

                if (modelType == ModelManager::ModelType::None) {
                    // do nothing
                } else if (modelType == ModelManager::ModelType::PakStaticMesh) {
                    if (pakModelDataList_.size() <= idx) {
                        pakModelDataList_.resize(idx + 1);
                        pakActorListOffsets.resize(idx + 1);
                    }
                    pakActorListOffsets[idx].push_back(actorListOffset);
                    pakModelDataList_[idx].numActors++;
                } else if (modelType == ModelManager::ModelType::Model) {
                    actorListOffsets[idx].push_back(actorListOffset);
                    modelDataList_[idx].numActors++;
                } else if (modelType == ModelManager::ModelType::SplineModel) {
                    std::unique_ptr<SplineData> s;
                    try {
                        s = std::make_unique<SplineData>(*proxy->getSaveObject());
                    } catch (const std::exception& ex) {
                        const auto& header = proxy->getSaveObject()->baseHeader();
                        spdlog::error("Error reading mSplineData: Class {}, Instance: {}", header.ClassName,
                            header.Reference.PathName);
                        continue;
                    }

                    // Copy spline segments to spline segment buffer, save position before and after
                    int32_t offset0 = static_cast<int32_t>(splineSegments[idx].size());
                    for (const auto& splineSegment : s->splineSegments()) {
                        splineSegments[idx].push_back(splineSegment);
                    }
                    int32_t offset1 = static_cast<int32_t>(splineSegments[idx].size());

                    // Calculate number of instances based on meshLength and lengthSum of all spline segments.
                    // Round to the nearest int, but at least one instance.
                    float meshLength = 2.0f; // TODO depends on mesh
                    int32_t numInstances = std::max(static_cast<int32_t>(std::lround(s->length() / meshLength)), 1);

                    for (int32_t i = 0; i < numInstances; i++) {
                        float t0 = s->length() * static_cast<float>(i) / static_cast<float>(numInstances);
                        float t1 = s->length() * static_cast<float>(i + 1) / static_cast<float>(numInstances);

                        const glm::vec3 forward =
                            determineInstanceForward(t0, t1, splineSegments[idx], offset0, offset1);

                        SplineMeshInstanceGpu instance{};
                        instance.listOffset = actorListOffset;
                        instance.offset0 = offset0;
                        instance.offset1 = offset1;
                        instance.t0 = t0;
                        instance.t1 = t1;
                        instance.forward = glm::vec4(forward, 1.0f);
                        splineInstances[idx].push_back(instance);
                        splineModelDataList_[idx].numInstances++;
                    }
                } else if (modelType == ModelManager::ModelType::PowerLine) {
                    std::vector<PowerLinePosition> positions;
                    try {
                        positions = getPowerLinePositions(*proxy->getSaveObject());
                    } catch (const std::exception& ex) {
                        const auto& header = proxy->getSaveObject()->baseHeader();
                        spdlog::error("Error reading mWireInstances: Class {}, Instance: {}", header.ClassName,
                            header.Reference.PathName);
                        continue;
                    }
                    for (const auto& pos : positions) {
                        // transform to [meter]
                        PowerLineInstanceGpu instance{
                            actorListOffset,
                            {0, 0, 0},
                            glm::vec4(pos.p0 * glm::vec3(0.01f), 1.0f),
                            glm::vec4(pos.p1 * glm::vec3(0.01f), 1.0f),
                        };
                        powerLineInstances.push_back(instance);
                        powerLineData_.numInstances++;
                    }
                }
            }
        }

        actorIdBuffer_.reset();
        actorTransformationBuffer_.reset();
        actorIdBuffer_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, actorIds);
        actorTransformationBuffer_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER,
            glm::value_ptr(actorTransformations.front()), actorTransformations.size() * sizeof(glm::mat4),
            GL_DYNAMIC_DRAW);

        for (std::size_t i = 0; i < pakModelDataList_.size(); i++) {
            auto& modelData = pakModelDataList_[i];
            modelData.listOffsetBuffer.reset();
            if (!pakActorListOffsets[i].empty()) {
                modelData.listOffsetBuffer =
                    std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, pakActorListOffsets[i]);
            }
        }

        for (std::size_t i = 0; i < modelDataList_.size(); i++) {
            auto& modelData = modelDataList_[i];
            modelData.listOffsetBuffer.reset();
            if (!actorListOffsets[i].empty()) {
                modelData.listOffsetBuffer =
                    std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, actorListOffsets[i]);
            }
        }

        for (std::size_t i = 0; i < splineModelDataList_.size(); i++) {
            auto& modelData = splineModelDataList_[i];
            modelData.splineSegments.reset();
            modelData.instanceData.reset();
            if (!splineSegments[i].empty() && !splineInstances[i].empty()) {
                modelData.splineSegments =
                    std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineSegments[i]);
                modelData.instanceData =
                    std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineInstances[i]);
            }
        }

        if (!powerLineInstances.empty()) {
            powerLineData_.instanceData =
                std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, powerLineInstances);
        }
    } catch (const std::exception& e) {
        spdlog::error("Error loading save: {}", e.what());
        showErrors_.push_back(std::string("Error loading save:\n") + e.what());
        savegame_.reset();
    }
}

void Satisfactory3DMap::DataView::addToNode(const ObjectProxyPtr& proxy, SaveNode& rootNode) {
    // Store objects into a tree structure for access by class
    std::reference_wrapper<SaveNode> n = rootNode;
    std::reference_wrapper<SaveNode> a_n = all_root_node_;
    for (const auto& s : s::splitPathName(proxy->className())) {
        n = n.get().childNodes[s];
        a_n = a_n.get().childNodes[s];
    }
    n.get().objects.emplace_back(proxy);
    a_n.get().objects.emplace_back(proxy);
}

void Satisfactory3DMap::DataView::saveSave(const std::filesystem::path& file) {
    if (std::filesystem::exists(file) && !std::filesystem::is_regular_file(file)) {
        spdlog::error("No regular file given!");
    }
    if (savegame_ != nullptr) {
        savegame_->save(file);
    }
}

void Satisfactory3DMap::DataView::selectPathName(const std::string& pathName) {
    if (savegame_ == nullptr) {
        return;
    }

    try {
        selectObject(savegame_->getObjectsByPath(pathName).back());
    } catch (...) {
        selectObject(-1);
    }
}

void Satisfactory3DMap::DataView::updateActor(const ObjectProxyPtr& actorProxy) {
    if (actorBufferPositions_.contains(actorProxy->id())) {
        const auto bufferPos = actorBufferPositions_.at(actorProxy->id());
        glm::mat4 transform = actorProxy->getTransformMat();
        actorTransformationBuffer_->bufferSubData(glm::value_ptr(transform), sizeof(glm::mat4),
            bufferPos * sizeof(glm::mat4));
    }
}
