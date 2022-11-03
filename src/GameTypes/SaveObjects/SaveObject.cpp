#include "SaveObject.h"

void Satisfactory3DMap::SaveObject::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << outer_path_name_;
}
