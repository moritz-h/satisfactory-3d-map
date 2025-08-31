#include "GameTypes/UE/Core/Serialization/MappedName.h"

#include "GameTypes/UE/Core/UObject/NameBatchSerialization.h"

void SatisfactorySave::FNameMap::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    NameEntries = LoadNameBatch(ar);
}
