#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FColor {
    public:
        uint8_t B = 0;
        uint8_t G = 0;
        uint8_t R = 0;
        uint8_t A = 0;

        void serialize(Archive& ar) {
            ar << B;
            ar << G;
            ar << R;
            ar << A;
        }
    };

    struct SATISFACTORYSAVE_API FLinearColor {
        float R = 0.0f;
        float G = 0.0f;
        float B = 0.0f;
        float A = 0.0f;

        void serialize(Archive& ar) {
            ar << R;
            ar << G;
            ar << B;
            ar << A;
        }
    };
} // namespace SatisfactorySave
