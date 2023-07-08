#include "Archive.h"

#include "GameTypes/Name.h"
#include "GameTypes/ObjectReference.h"

void Satisfactory3DMap::Archive::serializeName(Satisfactory3DMap::FName& n) {
    serializeString(n.Name);
    n.Number = 0;
}

void Satisfactory3DMap::Archive::serializeObjectReference(Satisfactory3DMap::ObjectReference& ref) {
    ref.serialize(*this);
}
