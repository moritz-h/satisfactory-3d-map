#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FObjectDataResource {
    public:
        uint32_t Flags = 0; // EObjectDataResourceFlags
        int64_t SerialOffset = -1;
        int64_t DuplicateSerialOffset = -1;
        int64_t SerialSize = -1;
        int64_t RawSize = -1;
        int32_t OuterIndex = 0; // FPackageIndex
        uint32_t LegacyBulkDataFlags = 0;

        void serialize(Archive& ar) {
            ar << Flags;
            ar << SerialOffset;
            ar << DuplicateSerialOffset;
            ar << SerialSize;
            ar << RawSize;
            ar << OuterIndex;
            ar << LegacyBulkDataFlags;
        }
    };

} // namespace SatisfactorySave
