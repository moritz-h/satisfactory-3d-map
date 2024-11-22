#pragma once

#include <variant>
#include <vector>

#include "Base/Property.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ByteProperty final : public PropertyImplBase<ByteProperty, std::variant<FName, int8_t>> {
    public:
        static constexpr std::string_view TypeName = "ByteProperty";

        using PropertyImplBase<ByteProperty, std::variant<FName, int8_t>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& EnumName() {
            return tag_.EnumName;
        }
    };
} // namespace SatisfactorySave
