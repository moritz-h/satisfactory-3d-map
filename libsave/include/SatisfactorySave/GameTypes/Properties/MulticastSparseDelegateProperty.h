#pragma once

#include "../UE/UObject/ScriptDelegates.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MulticastSparseDelegateProperty
        : public PropertyImpl<MulticastSparseDelegateProperty, FMulticastScriptDelegate> {

    public:
        static constexpr std::string_view TypeName = "MulticastSparseDelegateProperty";

        using PropertyImpl<MulticastSparseDelegateProperty, FMulticastScriptDelegate>::PropertyImpl;
    };
} // namespace SatisfactorySave
