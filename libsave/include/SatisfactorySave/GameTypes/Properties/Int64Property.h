#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Int64Property final : public PropertyImpl<Int64Property, int64_t> {
    public:
        static constexpr std::string_view TypeName = "Int64Property";

        using PropertyImpl<Int64Property, int64_t>::PropertyImpl;
    };
} // namespace SatisfactorySave
