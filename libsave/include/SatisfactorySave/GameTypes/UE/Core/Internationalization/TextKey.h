#pragma once

#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FTextKey {
    public:
        std::string Str;

        void serialize(Archive& ar) {
            // FTextKey::SerializeAsString is using a custom string serialization (TextKeyUtil::SaveKeyString).
            // This code always writes a null termintor, also for empty strings. Do the same for binary compatibility.
            static int32_t one = 1;
            static int8_t zero = 0;

            if (ar.isIArchive() || !Str.empty()) {
                ar << Str;
            } else {
                ar << one;
                ar << zero;
            }
        }
    };
} // namespace SatisfactorySave
