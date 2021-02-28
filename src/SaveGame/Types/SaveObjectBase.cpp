#include "SaveObjectBase.h"

#include "../Utils/StreamUtils.h"

SatisfactorySaveGame::SaveObjectBase::SaveObjectBase(int32_t type, std::istream& stream)
    : type_(type), properties_(nullptr) {
    class_name_ = read_length_string(stream);
    reference_ = ObjectReference(stream);
}

void SatisfactorySaveGame::SaveObjectBase::parseData(int32_t length, std::istream& stream) {
    properties_ = std::make_unique<PropertyCollection>(length, stream);
}
