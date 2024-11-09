#include "IO/Archive/Archive.h"

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/UE/Core/UObject/NameTypes.h"

void SatisfactorySave::Archive::serializeName(FName& n) {
    serializeString(n.Name);
    n.Number = 0;
}

void SatisfactorySave::Archive::serializeObjectReference(FObjectReferenceDisc& ref) {
    ref.serialize(*this);
}
