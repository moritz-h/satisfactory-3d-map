#include "SaveObjectBase.h"

#include "../Utils/FileUtils.h"

SatisfactorySaveGame::SaveObjectBase::SaveObjectBase(int32_t type, std::istream& stream) : type_(type) {
    class_name_ = read_length_string(stream);
    reference_.level_name = read_length_string(stream);
    reference_.path_name = read_length_string(stream);
}
