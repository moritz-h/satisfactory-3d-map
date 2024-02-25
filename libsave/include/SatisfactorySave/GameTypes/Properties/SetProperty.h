#pragma once

#include "../Sets/Base/Set.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SetProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "SetProperty";

        SetProperty(PropertyTag tag);

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline FName& setType() {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Set>& set() const {
            return set_;
        }

    protected:
        std::unique_ptr<Set> set_;
    };
} // namespace SatisfactorySave
