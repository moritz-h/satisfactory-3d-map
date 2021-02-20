#include "SaveObject.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveObject::SaveObject(int32_t type, std::istream& stream) : SaveObjectBase(type, stream) {
    parent_entity_name_ = read_length_string(stream);
}
