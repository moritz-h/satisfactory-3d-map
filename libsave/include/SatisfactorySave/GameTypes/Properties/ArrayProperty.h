#pragma once

#include <concepts>

#include "../Arrays/Base/Array.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API ArrayProperty final : public PropertyImplBase<ArrayProperty, std::shared_ptr<Array>> {
    public:
        static constexpr std::string_view TypeName = "ArrayProperty";

        using PropertyImplBase<ArrayProperty, std::shared_ptr<Array>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& ArrayType() {
            return tag_.InnerType;
        }

        template<std::derived_from<Array> T>
        inline T& get() const {
            if (const auto val = std::dynamic_pointer_cast<T>(Value); val != nullptr) {
                return *val;
            }
            throw std::runtime_error("Array type invalid: " + std::string(T::TypeName));
        }
    };
} // namespace SatisfactorySave
