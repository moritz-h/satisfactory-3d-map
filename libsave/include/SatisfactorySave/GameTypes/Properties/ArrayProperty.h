#pragma once

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
    };
} // namespace SatisfactorySave
