#pragma once

#include "../FactoryGame/FGObjectReference.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API InterfaceArray final : public ArrayImpl<InterfaceArray, FObjectReferenceDisc> {
    public:
        static constexpr std::string_view TypeName = "InterfaceProperty";
    };
} // namespace SatisfactorySave
