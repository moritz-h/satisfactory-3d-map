#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FRichCurveKey {
    public:
        /*TEnumAsByte<ERichCurveInterpMode>*/ uint8_t InterpMode;
        /*TEnumAsByte<ERichCurveTangentMode>*/ uint8_t TangentMode;
        /*TEnumAsByte<ERichCurveTangentWeightMode>*/ uint8_t TangentWeightMode;
        float Time;
        float Value;
        float ArriveTangent;
        float ArriveTangentWeight;
        float LeaveTangent;
        float LeaveTangentWeight;

        void serialize(Archive& ar) {
            ar << InterpMode;
            ar << TangentMode;
            ar << TangentWeightMode;
            ar << Time;
            ar << Value;
            ar << ArriveTangent;
            ar << ArriveTangentWeight;
            ar << LeaveTangent;
            ar << LeaveTangentWeight;
        }
    };
} // namespace SatisfactorySave
