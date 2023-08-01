#pragma once

#include "../UE/UObject/Name.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class NameProperty : public PropertyImpl<NameProperty, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";

        using PropertyImpl<NameProperty, FName>::PropertyImpl;
    };
} // namespace SatisfactorySave
