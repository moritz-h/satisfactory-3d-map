#include "SaveComponent.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveComponent::SaveComponent(int32_t type, std::istream& stream) : SaveObject(type, stream) {
    parent_entity_name_ = read_length_string(stream);
}
