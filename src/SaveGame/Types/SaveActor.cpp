#include "SaveActor.h"

#include "../Utils/StreamUtils.h"

SatisfactorySaveGame::SaveActor::SaveActor(int32_t type, std::istream& stream) : SaveObjectBase(type, stream) {
    need_transform_ = read<int32_t>(stream);
    rotation_ = read<Vec4>(stream);
    position_ = read<Vec3>(stream);
    scale_ = read<Vec3>(stream);
    was_placed_in_level_ = read<int32_t>(stream);
}

void SatisfactorySaveGame::SaveActor::parseData(int32_t length, std::istream& stream) {
    auto pos_before = stream.tellg();

    ObjectReference ref;
    ref.level_name = read_length_string(stream);
    ref.path_name = read_length_string(stream);

    auto count = read<int32_t>(stream);
    for (int i = 0; i < count; ++i) {
        ObjectReference child_ref;
        child_ref.level_name = read_length_string(stream);
        child_ref.path_name = read_length_string(stream);
    }

    auto pos_after = stream.tellg();

    SaveObjectBase::parseData(length - (pos_after - pos_before), stream);
}
