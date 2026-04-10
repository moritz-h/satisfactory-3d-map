#pragma once

#include <concepts>

#include "../Sets/Base/Set.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API SetProperty final : public PropertyImplBase<SetProperty, std::shared_ptr<Set>> {
    public:
        static constexpr std::string_view TypeName = "SetProperty";

        using PropertyImplBase<SetProperty, std::shared_ptr<Set>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& SetType() {
            return tag_.InnerType;
        }

        template<std::derived_from<Set> T>
        inline T& get() const {
            if (const auto val = std::dynamic_pointer_cast<T>(Value); val != nullptr) {
                return *val;
            }
            throw std::runtime_error("Set type invalid: " + std::string(T::TypeName));
        }
    };
} // namespace SatisfactorySave
