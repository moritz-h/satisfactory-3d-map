#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
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
        int64_t DuplicateSerialOffset = -1;

        [[nodiscard]] inline bool HasAnyFlags(EBulkDataFlags f) const {
            return (Flags & f) != 0;
        }
    };

    class SATISFACTORYSAVE_API FBulkData {
    public:
        FBulkMeta BulkMeta;
        std::vector<char> data;

        void serialize(Archive& ar);
    };

    using FByteBulkData = FBulkData;

} // namespace SatisfactorySave
