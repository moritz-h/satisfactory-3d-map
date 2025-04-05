#include "GameTypes/FactoryGame/FGSaveSession.h"

#include "../Save/SerializationUtils.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::FPerStreamingLevelSaveData::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        SaveVersion = ar.getSaveVersion(); // Default to SaveVersion from header

        auto& inAr = dynamic_cast<IStreamArchive&>(ar);
        // The game reads TOC and data as buffers, then reads SaveVersion below and then parses the buffers.
        // Here, seek and read SaveVersion in advance and then directly parse object data.
        if (inAr.getSaveVersion() >= 51) {
            auto pos_before = inAr.tell();
            const int64_t TOC_size = inAr.read<int64_t>();
            inAr.seek(ar.tell() + TOC_size);
            const int64_t data_size = inAr.read<int64_t>();
            inAr.seek(ar.tell() + data_size);
            inAr << SaveVersion;
            inAr.seek(pos_before);
        }
        auto save_version_stack_pusher = inAr.pushSaveVersion(SaveVersion);
        parseTOCBlob(inAr, SaveObjects, TOC_DestroyedActors);
        parseDataBlob(inAr, SaveObjects);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);
        auto save_version_stack_pusher = outAr.pushSaveVersion(SaveVersion);
        saveTOCBlob(outAr, SaveObjects, TOC_DestroyedActors);
        saveDataBlob(outAr, SaveObjects);
    }
    if (ar.getSaveVersion() >= 51) {
        ar << SaveVersion;
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
