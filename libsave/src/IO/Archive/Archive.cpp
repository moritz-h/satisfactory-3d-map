#include "IO/Archive/Archive.h"

#include "GameTypes/UE/Satisfactory/ObjectReference.h"
#include "GameTypes/UE/UObject/Name.h"

void SatisfactorySave::Archive::serializeName(SatisfactorySave::FName& n) {
    serializeString(n.Name);
    n.Number = 0;
}

void SatisfactorySave::Archive::serializeObjectReference(SatisfactorySave::FObjectReferenceDisc& ref) {
    ref.serialize(*this);
}
