#include "GameTypes/Misc/SaveObject.h"

void SatisfactorySave::SaveObject::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << outer_path_name_;
}