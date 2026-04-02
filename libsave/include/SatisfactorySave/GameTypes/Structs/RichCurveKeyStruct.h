#pragma once

#include "../UE/Engine/Curves/RichCurve.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API RichCurveKeyStruct final : public StructImpl<RichCurveKeyStruct, FRichCurveKey> {
    public:
        static constexpr std::string_view TypeName = "RichCurveKey";

        using StructImpl<RichCurveKeyStruct, FRichCurveKey>::StructImpl;
    };
} // namespace SatisfactorySave
