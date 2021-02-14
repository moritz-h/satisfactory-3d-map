#include "SaveEntity.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveEntity::SaveEntity(int32_t type, std::istream& stream) : SaveObject(type, stream) {
    need_transform_ = read<int32_t>(stream);
    rotation_ = read<Vec4>(stream);
    position_ = read<Vec3>(stream);
    scale_ = read<Vec3>(stream);
    was_placed_in_level_ = read<int32_t>(stream);
}
