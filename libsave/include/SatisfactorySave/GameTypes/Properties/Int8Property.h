#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Int8Property final : public PropertyImpl<Int8Property, int8_t> {
    public:
        static constexpr std::string_view TypeName = "Int8Property";

        using PropertyImpl<Int8Property, int8_t>::PropertyImpl;
    };
} // namespace SatisfactorySave
