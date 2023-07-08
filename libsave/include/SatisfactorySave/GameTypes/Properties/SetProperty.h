#pragma once

#include "../Sets/Set.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class SetProperty : public Property {
    public:
        SetProperty(PropertyTag tag, std::string parentClassName);

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& setType() const {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Set>& set() const {
            return set_;
        }

    protected:
        std::string parentClassName_;
        std::unique_ptr<Set> set_;
    };
} // namespace Satisfactory3DMap
