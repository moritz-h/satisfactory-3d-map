#include "SaveObject.h"

#include "Utils/StreamUtils.h"

Satisfactory3DMap::SaveObject::SaveObject(int32_t type, std::istream& stream) : SaveObjectBase(type, stream) {
    outer_path_name_ = read_length_string(stream);
}
