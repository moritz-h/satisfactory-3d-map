#include "GameTypes/UE/Core/Serialization/MappedName.h"

#include "GameTypes/UE/Core/UObject/NameBatchSerialization.h"

void SatisfactorySave::FNameMap::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    NameEntries = LoadNameBatch(ar);
}

std::string SatisfactorySave::FNameMap::toString() const {
    std::stringstream result;
    int i = 0;
    for (const auto& entry : NameEntries) {
        result << std::hex << std::setfill('0') << std::setw(2) << i << " " << entry << std::endl;
        i++;
    }
    return result.str();
}
