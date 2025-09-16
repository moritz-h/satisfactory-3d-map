#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../../../IO/Archive/IStreamArchive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API TResourceArray {
        int32_t SerializedElementSize = 0;
        int32_t Num = 0;
        std::vector<char> data;

        void serialize(Archive& ar) {
            if (!ar.isIArchive()) {
                throw std::runtime_error("Only IStreamArchive support implemented!");
            }

            // BulkSerialize
            ar << SerializedElementSize;
            ar << Num;

            auto& inAr = dynamic_cast<IStreamArchive&>(ar);
            data = inAr.read_buffer(SerializedElementSize * Num);
        }
    };
} // namespace SatisfactorySave
