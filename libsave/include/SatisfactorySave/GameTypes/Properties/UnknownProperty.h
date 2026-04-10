#pragma once

#include <cstddef>
#include <vector>

#include "Base/Property.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API UnknownProperty final : public Property {
    public:
        using Property::Property;

        [[nodiscard]] std::shared_ptr<Property> clone() const override;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        std::vector<std::byte> Value;
    };
} // namespace SatisfactorySave
