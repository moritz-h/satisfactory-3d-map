#include "SaveActor.h"

#include <glm/ext/matrix_transform.hpp>

#include "IO/Archive/OStreamArchive.h"

void Satisfactory3DMap::SaveActor::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << need_transform_;
    ar << rotation_;
    ar << position_;
    ar << scale_;
    ar << was_placed_in_level_;
}

void Satisfactory3DMap::SaveActor::serializeProperties(Satisfactory3DMap::Archive& ar, int32_t length) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        auto pos_before = inAr.tell();

        inAr << parent_reference_;
        auto count = inAr.read<int32_t>();
        child_references_.resize(count);
        for (int i = 0; i < count; ++i) {
            inAr << child_references_[i];
        }

        auto pos_after = inAr.tell();

        SaveObjectBase::serializeProperties(inAr, length - static_cast<int32_t>(pos_after - pos_before));
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr << parent_reference_;
        outAr.write(static_cast<int32_t>(child_references_.size()));
        for (auto& ref : child_references_) {
            outAr << ref;
        }
        SaveObjectBase::serializeProperties(outAr, 0);
    }
}

glm::mat4 Satisfactory3DMap::SaveActor::transformation() const {
    const auto translation = glm::translate(glm::mat4(1.0f), position_ * glm::vec3(0.01f, -0.01f, 0.01f));
    const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
    const auto scale = glm::scale(glm::mat4(1.0f), scale_);
    return translation * rotation * scale;
}
