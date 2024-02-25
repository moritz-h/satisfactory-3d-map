#pragma once

#include "../UE/UObject/SoftObjectPath.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftObjectProperty : public PropertyImpl<SoftObjectProperty, FSoftObjectPath> {
    public:
        static constexpr std::string_view TypeName = "SoftObjectProperty";

        using PropertyImpl<SoftObjectProperty, FSoftObjectPath>::PropertyImpl;
    };
} // namespace SatisfactorySave
