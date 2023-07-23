#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class UInt32Property : public PropertyImpl<UInt32Property, uint32_t> {
    public:
        static constexpr std::string_view TypeName = "UInt32Property";

        using PropertyImpl<UInt32Property, uint32_t>::PropertyImpl;
    };
} // namespace SatisfactorySave
