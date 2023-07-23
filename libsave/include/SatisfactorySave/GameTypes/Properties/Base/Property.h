#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "../../../IO/Archive/IStreamArchive.h"
#include "PropertyTag.h"

namespace SatisfactorySave {

    class PropertyVisitor;

    class Property {
    public:
        static std::unique_ptr<Property> create(IStreamArchive& ar, const std::string& parentClassName);

        explicit Property(PropertyTag tag);
        virtual ~Property() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(PropertyVisitor& v) = 0;

        PropertyTag Tag;
    };
} // namespace SatisfactorySave
