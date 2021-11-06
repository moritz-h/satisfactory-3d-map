#include "SaveActor.h"

#include <glm/ext/matrix_transform.hpp>

#include "Utils/StreamUtils.h"

Satisfactory3DMap::SaveActor::SaveActor(int32_t id, int32_t type, std::istream& stream)
    : SaveObjectBase(id, type, stream),
      parent_reference_(nullptr),
      child_references_(nullptr) {
    need_transform_ = read<int32_t>(stream);
    rotation_ = read<glm::quat>(stream);
    position_ = read<glm::vec3>(stream);
    scale_ = read<glm::vec3>(stream);
    was_placed_in_level_ = read<int32_t>(stream);
}

void Satisfactory3DMap::SaveActor::serialize(std::ostream& stream) const {
    SaveObjectBase::serialize(stream);
    write(stream, need_transform_);
    write(stream, rotation_);
    write(stream, position_);
    write(stream, scale_);
    write(stream, was_placed_in_level_);
}

void Satisfactory3DMap::SaveActor::parseData(int32_t length, std::istream& stream) {
    auto pos_before = stream.tellg();

    parent_reference_ = std::make_unique<ObjectReference>(stream);
    child_references_ = std::make_unique<std::vector<ObjectReference>>();
    auto count = read<int32_t>(stream);
    for (int i = 0; i < count; ++i) {
        child_references_->emplace_back(stream);
    }

    auto pos_after = stream.tellg();

    SaveObjectBase::parseData(length - static_cast<int32_t>(pos_after - pos_before), stream);
}

glm::mat4 Satisfactory3DMap::SaveActor::transformation() const {
    const auto translation = glm::translate(glm::mat4(1.0f), position_ * glm::vec3(0.01f, -0.01f, 0.01f));
    const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
    const auto scale = glm::scale(glm::mat4(1.0f), scale_);
    return translation * rotation * scale;
}
