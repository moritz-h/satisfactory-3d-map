#pragma once

#include "../Arrays/Base/Array.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ArrayProperty final : public PropertyImplBase<ArrayProperty, std::unique_ptr<Array>> {
    public:
        static constexpr std::string_view TypeName = "ArrayProperty";

        using PropertyImplBase<ArrayProperty, std::unique_ptr<Array>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& arrayType() {
            return tag_.InnerType;
        }
    };
} // namespace SatisfactorySave
