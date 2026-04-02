#pragma once

#include <concepts>
#include <memory>

#include "../Structs/Base/Struct.h"
#include "../UE/Core/Misc/Guid.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructProperty final : public PropertyImplBase<StructProperty, std::shared_ptr<Struct>> {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        using PropertyImplBase<StructProperty, std::shared_ptr<Struct>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& StructName() {
            return tag_.StructName;
        }

        [[nodiscard]] inline FGuid& StructGuid() {
            return tag_.StructGuid;
        }

        template<std::derived_from<Struct> T>
        inline T& get() const {
            if (const auto val = std::dynamic_pointer_cast<T>(Value); val != nullptr) {
                return *val;
            }
            throw std::runtime_error("Struct type invalid: " + std::string(T::TypeName));
        }
    };
} // namespace SatisfactorySave
