#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "../../../IO/Archive/IStreamArchive.h"
#include "PropertyTag.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class PropertyVisitor;

    class SATISFACTORYSAVE_API Property {
    public:
        static std::unique_ptr<Property> create(IStreamArchive& ar, const std::string& parentClassName);

        explicit Property(PropertyTag tag);
        virtual ~Property() = default;

        // Delete copy operators, to allow use of unique_ptr.
        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;
        Property(Property&&) = default;
        Property& operator=(Property&&) = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(PropertyVisitor& v) = 0;

        PropertyTag Tag;
    };
} // namespace SatisfactorySave
