#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <glowl/glowl.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/Pak/PakManager.h"

#include "../Config/Configuration.h"
#include "../Config/PathSetting.h"
#include "ModelManager.h"
#include "ObjectProxy.h"

namespace Satisfactory3DMap {

    class DataView {
    public:
        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            ObjectProxyList objects;
            std::size_t numObjects = 0;
            std::size_t numActors = 0;
        };

        struct ModelData {
            ModelData() : numActors(0) {}
            std::unique_ptr<glowl::BufferObject> listOffsetBuffer;
            int numActors;
        };
        struct SplineModelData {
            SplineModelData() : numInstances(0) {}
            std::unique_ptr<glowl::BufferObject> splineSegments;
            std::unique_ptr<glowl::BufferObject> instanceData;
            int numInstances;
        };

        explicit DataView(std::shared_ptr<Configuration> config);

        void openSave(const std::filesystem::path& file);
        void saveSave(const std::filesystem::path& file);

        [[nodiscard]] inline bool hasSave() const {
            return savegame_ != nullptr;
        }

        [[nodiscard]] inline const std::unique_ptr<SatisfactorySave::SaveGame>& saveGame() const {
            return savegame_;
        }

        [[nodiscard]] const std::vector<SaveNode>& levelRootNodes() const {
            return level_root_nodes_;
        }

        [[nodiscard]] const SaveNode& persistentAndRuntimeRootNode() const {
            return persistent_and_runtime_root_node_;
        }

        [[nodiscard]] const SaveNode& lightweightBuildableRootNode() const {
            return lightweight_buildable_root_node_;
        }

        [[nodiscard]] const SaveNode& allRootNode() const {
            return all_root_node_;
        }

        void selectPathName(const std::string& pathName);

        void selectObject(int32_t id) {
            if (id >= 0 && id < static_cast<int32_t>(proxy_list_.size())) {
                selectedObject_ = proxy_list_[id];
            } else {
                selectedObject_ = nullptr;
            }
        }

        void selectObject(const SatisfactorySave::SaveObjectPtr& obj) {
            if (object_proxy_map_.contains(obj)) {
                selectedObject_ = object_proxy_map_.at(obj);
            } else {
                selectedObject_ = nullptr;
            }
        }

        void selectObject(const ObjectProxyPtr& obj) {
            selectedObject_ = obj;
        }

        [[nodiscard]] inline bool hasSelectedObject() const {
            return selectedObject_ != nullptr;
        }

        [[nodiscard]] inline const ObjectProxyPtr& selectedObject() const {
            return selectedObject_;
        }

        [[nodiscard]] inline int32_t selectedObjectId() const {
            return (selectedObject() != nullptr) ? selectedObject()->id() : -1;
        }

        void updateActor(const ObjectProxyPtr& actorProxy);

        [[nodiscard]] const std::shared_ptr<SatisfactorySave::PakManager>& pakManager() const {
            return pakManager_;
        }

        // TODO remove from interface
        [[nodiscard]] const std::unique_ptr<ModelManager>& manager() const {
            return manager_;
        }

        [[nodiscard]] const std::unique_ptr<glowl::BufferObject>& actorIdBuffer() const {
            return actorIdBuffer_;
        }

        [[nodiscard]] const std::unique_ptr<glowl::BufferObject>& actorTransformationBuffer() const {
            return actorTransformationBuffer_;
        }

        [[nodiscard]] const std::vector<ModelData>& pakModelDataList() const {
            return pakModelDataList_;
        }

        [[nodiscard]] const std::vector<ModelData>& modelDataList() const {
            return modelDataList_;
        }

        [[nodiscard]] const std::vector<SplineModelData>& splineModelDataList() const {
            return splineModelDataList_;
        }

        std::deque<std::string>& showErrors() {
            return showErrors_;
        }

    protected:
        void addToNode(const ObjectProxyPtr& proxy, SaveNode& rootNode);

        std::shared_ptr<Configuration> config_;
        std::shared_ptr<PathSetting> gameDirSetting_;

        std::shared_ptr<SatisfactorySave::PakManager> pakManager_;
        std::unique_ptr<ModelManager> manager_;

        std::unique_ptr<SatisfactorySave::SaveGame> savegame_;
        ObjectProxyList proxy_list_;
        std::unordered_map<SatisfactorySave::SaveObjectPtr, ObjectProxyPtr> object_proxy_map_;
        ObjectProxyList lightweight_buildable_list_;
        std::vector<SaveNode> level_root_nodes_;
        SaveNode persistent_and_runtime_root_node_;
        SaveNode lightweight_buildable_root_node_;
        SaveNode all_root_node_;
        std::shared_ptr<SatisfactorySave::AFGLightweightBuildableSubsystem> lightweight_buildable_subsystem_;

        ObjectProxyPtr selectedObject_;

        std::unique_ptr<glowl::BufferObject> actorIdBuffer_;
        std::unique_ptr<glowl::BufferObject> actorTransformationBuffer_;

        std::vector<ModelData> pakModelDataList_;
        std::vector<ModelData> modelDataList_;
        std::vector<SplineModelData> splineModelDataList_;

        std::unordered_map<int32_t, int32_t> actorBufferPositions_;

        std::deque<std::string> showErrors_;
    };
} // namespace Satisfactory3DMap
