#include "SaveActor.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveActor::SaveActor(int32_t type, std::istream& stream) : SaveObjectBase(type, stream) {
    need_transform_ = read<int32_t>(stream);
    rotation_ = read<Vec4>(stream);
    position_ = read<Vec3>(stream);
    scale_ = read<Vec3>(stream);
    was_placed_in_level_ = read<int32_t>(stream);
}

void SatisfactorySaveGame::SaveActor::parseData(int32_t length, std::istream& stream) {
    stream.ignore(length);
}
