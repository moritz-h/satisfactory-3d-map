#pragma once

#include "../UE/Core/UObject/ScriptDelegates.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MulticastSparseDelegateProperty final
        : public PropertyImpl<MulticastSparseDelegateProperty, FMulticastScriptDelegate> {

    public:
        static constexpr std::string_view TypeName = "MulticastSparseDelegateProperty";

        using PropertyImpl<MulticastSparseDelegateProperty, FMulticastScriptDelegate>::PropertyImpl;
    };
} // namespace SatisfactorySave
