#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FActorSaveHeader
    class SaveActor : public SaveObjectBase {
    public:
        using SaveObjectBase::SaveObjectBase;

        void serialize(Archive& ar) override;

        void serializeProperties(Archive& ar, int32_t length) override;

        [[nodiscard]] glm::mat4 transformation() const;

        [[nodiscard]] const glm::quat& rotation() const {
            return rotation_;
        }

        [[nodiscard]] glm::quat& rotation() {
            return rotation_;
        }

        [[nodiscard]] const glm::vec3& position() const {
            return position_;
        }

        [[nodiscard]] glm::vec3& position() {
            return position_;
        }

        [[nodiscard]] const glm::vec3& scale() const {
            return scale_;
        }

        [[nodiscard]] glm::vec3& scale() {
            return scale_;
        }

        [[nodiscard]] int32_t needTransform() const {
            return need_transform_;
        }

        [[nodiscard]] int32_t wasPlacedInLevel() const {
            return was_placed_in_level_;
        };

        [[nodiscard]] const ObjectReference& parentReference() const {
            return parent_reference_;
        }

        [[nodiscard]] const std::vector<ObjectReference>& childReferences() const {
            return child_references_;
        }

    protected:
        // Type size validation
        static_assert(sizeof(glm::quat) == 4 * sizeof(float));
        static_assert(sizeof(glm::vec3) == 3 * sizeof(float));

        int32_t need_transform_ = 0;
        glm::quat rotation_ = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 position_ = glm::vec3(0.0f);
        glm::vec3 scale_ = glm::vec3(0.0f);
        int32_t was_placed_in_level_ = 0;

        ObjectReference parent_reference_;
        std::vector<ObjectReference> child_references_;
    };
} // namespace SatisfactorySave
