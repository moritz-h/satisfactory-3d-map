#pragma once

#include "../FactoryGame/FGObjectReference.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ObjectArray final : public ArrayImpl<ObjectArray, FObjectReferenceDisc> {
    public:
        static constexpr std::string_view TypeName = "ObjectProperty";
    };
} // namespace SatisfactorySave
