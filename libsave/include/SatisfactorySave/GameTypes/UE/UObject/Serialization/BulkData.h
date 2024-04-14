#pragma once

#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../../../Pak/AssetFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    enum EBulkDataFlags : uint32_t {
        BULKDATA_None = 0,
        BULKDATA_PayloadAtEndOfFile = 1 << 0,
        BULKDATA_SerializeCompressedZLIB = 1 << 1,
        BULKDATA_ForceSingleElementSerialization = 1 << 2,
        BULKDATA_SingleUse = 1 << 3,
        BULKDATA_Unused = 1 << 5,
        BULKDATA_ForceInlinePayload = 1 << 6,
        BULKDATA_SerializeCompressed = (BULKDATA_SerializeCompressedZLIB),
        BULKDATA_ForceStreamPayload = 1 << 7,
        BULKDATA_PayloadInSeperateFile = 1 << 8,
        BULKDATA_SerializeCompressedBitWindow = 1 << 9,
        BULKDATA_Force_NOT_InlinePayload = 1 << 10,
        BULKDATA_OptionalPayload = 1 << 11,
        BULKDATA_MemoryMappedPayload = 1 << 12,
        BULKDATA_Size64Bit = 1 << 13,
        BULKDATA_DuplicateNonOptionalPayload = 1 << 14,
        BULKDATA_BadDataVersion = 1 << 15,
        BULKDATA_NoOffsetFixUp = 1 << 16,
        BULKDATA_WorkspaceDomainPayload = 1 << 17,
        BULKDATA_LazyLoadable = 1 << 18,
        BULKDATA_UsesIoDispatcher = 1u << 31u,
        BULKDATA_DataIsMemoryMapped = 1 << 30,
        BULKDATA_HasAsyncReadPending = 1 << 29,
        BULKDATA_AlwaysAllowDiscard = 1 << 28,
    };

    struct SATISFACTORYSAVE_API FBulkMeta {
    public:
        EBulkDataFlags Flags = BULKDATA_None;
        int64_t Offset = 0;
        int64_t Size = 0;
        int64_t SizeOnDisk = 0;
        int64_t DuplicateSerialOffset = -1;

        [[nodiscard]] inline bool HasAnyFlags(EBulkDataFlags f) const {
            return (Flags & f) != 0;
        }
    };

    class SATISFACTORYSAVE_API FBulkData {
    public:
        FBulkMeta BulkMeta;
        std::vector<char> data;

        void serialize(Archive& ar) {
            // FBulkData::Serialize allows the archive to provide an alternative serialization, see
            // https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/CoreUObject/Private/Serialization/BulkData.cpp#L1126
            // This is done by FLinkerLoad.

            auto assetAr = dynamic_cast<AssetFile*>(&ar);
            if (assetAr == nullptr) {
                throw std::runtime_error("FBulkData serialization outside AssetFile not implemented!");
            }

            if (assetAr->DataResourceMap.empty()) {
                throw std::runtime_error("FBulkData: DataResourceMap empty not implemented!");
            }

            int32_t DataResourceIndex = 0;
            *assetAr << DataResourceIndex;
            const auto& DataResource = assetAr->DataResourceMap.at(DataResourceIndex);

            BulkMeta.Flags = static_cast<EBulkDataFlags>(DataResource.LegacyBulkDataFlags);
            BulkMeta.Offset = DataResource.SerialOffset;
            BulkMeta.Size = DataResource.RawSize;
            BulkMeta.SizeOnDisk = DataResource.SerialSize;
            BulkMeta.DuplicateSerialOffset = DataResource.DuplicateSerialOffset;

            if (BulkMeta.HasAnyFlags(BULKDATA_SerializeCompressed)) {
                throw std::runtime_error("FBulkData: BULKDATA_SerializeCompressed not implemented!");
            }

            const bool bIsInline = BulkMeta.HasAnyFlags(BULKDATA_PayloadAtEndOfFile) == false;
            if (bIsInline) {
                data = assetAr->read_buffer(BulkMeta.Size);
            } else if (BulkMeta.HasAnyFlags(BULKDATA_PayloadInSeperateFile)) {
                const auto pos_before = assetAr->tell();
                assetAr->seek(BulkMeta.Offset + assetAr->summary().BulkDataStartOffset);
                data = assetAr->read_buffer(BulkMeta.Size);
                assetAr->seek(pos_before);
            } else {
                // editor only?
                throw std::runtime_error("FBulkData: not implemented!");
            }
        }
    };

    using FByteBulkData = FBulkData;

} // namespace SatisfactorySave
