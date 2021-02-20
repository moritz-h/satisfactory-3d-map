#include "SaveObjectBase.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveObjectBase::SaveObjectBase(int32_t type, std::istream& stream) : type_(type) {
    type_path_ = read_length_string(stream);
    root_object_ = read_length_string(stream);
    instance_name_ = read_length_string(stream);
}
