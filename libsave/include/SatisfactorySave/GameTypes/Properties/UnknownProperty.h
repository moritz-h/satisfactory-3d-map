#pragma once

#include <vector>

#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UnknownProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::vector<char>& value() const {
            return value_;
        }

    protected:
        std::vector<char> value_;
    };
} // namespace SatisfactorySave
