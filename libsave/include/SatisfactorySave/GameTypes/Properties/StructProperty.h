#pragma once

#include <vector>

#include "../Structs/Base/Struct.h"
#include "../UE/Misc/Guid.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructProperty final : public PropertyImplBase<StructProperty, std::unique_ptr<Struct>> {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        using PropertyImplBase<StructProperty, std::unique_ptr<Struct>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& structName() {
            return tag_.StructName;
        }

        [[nodiscard]] inline FGuid& structGuid() {
            return tag_.StructGuid;
        }
    };
} // namespace SatisfactorySave
