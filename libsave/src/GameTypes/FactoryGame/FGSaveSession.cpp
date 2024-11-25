#include "GameTypes/FactoryGame/FGSaveSession.h"

#include "../Save/SerializationUtils.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::FPerStreamingLevelSaveData::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);
        parseTOCBlob(inAr, SaveObjects, TOC_DestroyedActors);
        parseDataBlob(inAr, SaveObjects);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);
        saveTOCBlob(outAr, SaveObjects, TOC_DestroyedActors);
        saveDataBlob(outAr, SaveObjects);
    }

    ar << DestroyedActors;
}

void SatisfactorySave::FPersistentAndRuntimeSaveData::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);
        parseTOCBlob(inAr, SaveObjects, TOC_LevelToDestroyedActorsMap);
        parseDataBlob(inAr, SaveObjects);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);
        saveTOCBlob(outAr, SaveObjects, TOC_LevelToDestroyedActorsMap);
        saveDataBlob(outAr, SaveObjects);
    }

    ar << LevelToDestroyedActorsMap;
}
