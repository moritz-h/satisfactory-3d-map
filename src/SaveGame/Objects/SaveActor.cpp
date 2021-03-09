#include "SaveActor.h"

#include "Utils/StreamUtils.h"

Satisfactory3DMap::SaveActor::SaveActor(int32_t type, std::istream& stream)
    : SaveObjectBase(type, stream), parent_reference_(nullptr), child_references_(nullptr) {
    need_transform_ = read<int32_t>(stream);
    rotation_ = read<Vec4>(stream);
    position_ = read<Vec3>(stream);
    scale_ = read<Vec3>(stream);
    was_placed_in_level_ = read<int32_t>(stream);
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
