#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/Save/SaveObject.h"

#include "Utils/GLMUtil.h"

namespace Satisfactory3DMap {
    class ObjectProxy {
    public:
        ObjectProxy(int32_t id, SatisfactorySave::SaveObjectPtr saveObject)
            : id_(id),
              saveObject_(std::move(saveObject)),
              lightweightIdx_(0),
              instanceData_(nullptr) {}

        ObjectProxy(int32_t id, std::string lightweightClassName, std::size_t lightweightIdx,
            SatisfactorySave::FRuntimeBuildableInstanceData* instanceData)
            : id_(id),
              lightweightClassName_(std::move(lightweightClassName)),
              lightweightIdx_(lightweightIdx),
              instanceData_(instanceData) {}

        [[nodiscard]] int32_t id() const {
            return id_;
        }

        [[nodiscard]] const std::string& className() const {
            if (saveObject_ != nullptr) {
                return saveObject_->baseHeader().ClassName;
            }
            return lightweightClassName_;
        }

        [[nodiscard]] std::string pathName() const {
            if (saveObject_ != nullptr) {
                return saveObject_->baseHeader().Reference.PathName;
            }
            return std::string("LightweightBuildable_") + std::to_string(lightweightIdx_);
        }

        [[nodiscard]] bool isLightweight() const {
            return saveObject_ == nullptr;
        }

        [[nodiscard]] bool isActor() const {
            if (saveObject_ != nullptr) {
                return saveObject_->isActor();
            }
            // A lightweight buildable is always an actor.
            return true;
        }

        [[nodiscard]] const SatisfactorySave::SaveObjectPtr& getSaveObject() const {
            if (saveObject_ == nullptr) {
                throw std::runtime_error("Is lightweight buildable!");
            }
            return saveObject_;
        }

        [[nodiscard]] glm::mat4 getTransformMat() const {
            if (saveObject_ != nullptr) {
                if (saveObject_->isActor()) {
                    return glmCast(saveObject_->actorHeader().Transform);
                }
                throw std::runtime_error("Not an actor!");
            }
            return glmCast(instanceData_->Transform);
        }

        [[nodiscard]] glm::vec3 getTranslation() const {
            if (saveObject_ != nullptr) {
                if (saveObject_->isActor()) {
                    return glmCast(saveObject_->actorHeader().Transform.Translation);
                }
                throw std::runtime_error("Not an actor!");
            }
            return glmCast(instanceData_->Transform.Translation);
        }

    private:
        int32_t id_;
        SatisfactorySave::SaveObjectPtr saveObject_;
        std::string lightweightClassName_;
        std::size_t lightweightIdx_;
        SatisfactorySave::FRuntimeBuildableInstanceData* instanceData_;
    };
    using ObjectProxyPtr = std::shared_ptr<ObjectProxy>;
    using ObjectProxyList = std::vector<ObjectProxyPtr>;
} // namespace Satisfactory3DMap
