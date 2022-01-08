#include "SaveObject.h"

Satisfactory3DMap::SaveObject::SaveObject(int32_t id) : SaveObjectBase(id) {}

void Satisfactory3DMap::SaveObject::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << outer_path_name_;
}
