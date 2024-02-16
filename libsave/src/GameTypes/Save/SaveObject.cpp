#include "GameTypes/Save/SaveObject.h"

SatisfactorySave::SaveObject::SaveObject() : SaveObjectBase(false) {}

void SatisfactorySave::SaveObject::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << OuterPathName;
}
