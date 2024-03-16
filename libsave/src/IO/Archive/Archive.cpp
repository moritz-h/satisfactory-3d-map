#include "IO/Archive/Archive.h"

#include "GameTypes/UE/Satisfactory/ObjectReference.h"
#include "GameTypes/UE/UObject/Name.h"

void SatisfactorySave::Archive::serializeName(FName& n) {
    serializeString(n.Name);
    n.Number = 0;
}

void SatisfactorySave::Archive::serializeObjectReference(FObjectReferenceDisc& ref) {
    ref.serialize(*this);
}
