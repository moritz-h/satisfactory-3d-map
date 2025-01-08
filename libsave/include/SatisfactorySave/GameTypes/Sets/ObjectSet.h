#pragma once

#include "../FactoryGame/FGObjectReference.h"
#include "Base/SetImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ObjectSet final : public SetImpl<ObjectSet, FObjectReferenceDisc> {
    public:
        static constexpr std::string_view TypeName = "ObjectProperty";
    };
} // namespace SatisfactorySave
