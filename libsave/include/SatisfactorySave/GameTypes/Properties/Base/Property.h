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

        explicit Property(FName type);
        explicit Property(PropertyTag tag);
        virtual ~Property() = default;

        // Delete copy operators, to allow use of unique_ptr.
        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;
        Property(Property&&) = default;
        Property& operator=(Property&&) = default;

        [[nodiscard]] inline FName& name() {
            return tag_.Name;
        }

        [[nodiscard]] inline const FName& type() const {
            return tag_.Type;
        }

        [[nodiscard]] inline int32_t& arrayIndex() {
            return tag_.ArrayIndex;
        }

        [[nodiscard]] inline uint8_t& hasPropertyGuid() {
            return tag_.HasPropertyGuid;
        }

        [[nodiscard]] inline FGuid& propertyGuid() {
            return tag_.PropertyGuid;
        }

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(PropertyVisitor& v) = 0;

    protected:
        PropertyTag tag_;

        friend class PropertyList;
    };
} // namespace SatisfactorySave
