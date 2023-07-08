#include "IO/Archive/Archive.h"

#include "GameTypes/Misc/Name.h"
#include "GameTypes/Misc/ObjectReference.h"

void SatisfactorySave::Archive::serializeName(SatisfactorySave::FName& n) {
    serializeString(n.Name);
    n.Number = 0;
}

void SatisfactorySave::Archive::serializeObjectReference(SatisfactorySave::ObjectReference& ref) {
    ref.serialize(*this);
}
