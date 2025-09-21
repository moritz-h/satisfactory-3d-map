#include "GameTypes/UE/CoreUObject/Serialization/BulkData.h"

#include "Pak/AssetFile.h"

void SatisfactorySave::FBulkData::serialize(Archive& ar) {
    // FBulkData::Serialize allows the archive to provide an alternative serialization, see
    // https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/CoreUObject/Private/Serialization/BulkData.cpp#L1126
    // This is done by FLinkerLoad.

    auto assetAr = dynamic_cast<AssetFile*>(&ar);
    if (assetAr == nullptr) {
        throw std::runtime_error("FBulkData serialization outside AssetFile not implemented!");
    }

    if (assetAr->bulkDataMap().empty()) {
        throw std::runtime_error("FBulkData: BulkDataMap empty not implemented!");
    }

    int32_t EntryIndex = 0;
    *assetAr << EntryIndex;
    const auto& Entry = assetAr->bulkDataMap().at(EntryIndex);

    BulkMeta.Flags = static_cast<EBulkDataFlags>(Entry.Flags);
    BulkMeta.Offset = Entry.SerialOffset;
    BulkMeta.Size = Entry.SerialSize;
    BulkMeta.DuplicateSerialOffset = Entry.DuplicateSerialOffset;

    if (BulkMeta.HasAnyFlags(BULKDATA_SerializeCompressed)) {
        throw std::runtime_error("FBulkData: BULKDATA_SerializeCompressed not implemented!");
    }

    const bool bIsInline = BulkMeta.HasAnyFlags(BULKDATA_PayloadAtEndOfFile) == false;
    if (bIsInline) {
        data = assetAr->read_buffer(BulkMeta.Size);
    } else if (BulkMeta.HasAnyFlags(BULKDATA_PayloadInSeperateFile)) {
        assetAr->ubulkAr().seek(BulkMeta.Offset);
        data = assetAr->ubulkAr().read_buffer(BulkMeta.Size);
    } else {
        // editor only?
        throw std::runtime_error("FBulkData: not implemented!");
    }
}
