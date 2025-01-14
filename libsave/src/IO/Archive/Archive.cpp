#include "IO/Archive/Archive.h"

#include "GameTypes/FactoryGame/FGObjectReference.h"

void SatisfactorySave::Archive::serializeObjectReference(FObjectReferenceDisc& ref) {
    ref.serialize(*this);
}
