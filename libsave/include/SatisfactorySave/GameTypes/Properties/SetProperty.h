#pragma once

#include "../Sets/Base/Set.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SetProperty final : public PropertyImplBase<SetProperty, std::unique_ptr<Set>> {
    public:
        static constexpr std::string_view TypeName = "SetProperty";

        using PropertyImplBase<SetProperty, std::unique_ptr<Set>>::PropertyImplBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] inline FName& setType() {
            return tag_.InnerType;
        }
    };
} // namespace SatisfactorySave
