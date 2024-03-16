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
        static std::unique_ptr<Property> create(IStreamArchive& ar);

        explicit Property(FName type);
        Property(FName type, FName name);
        explicit Property(PropertyTag tag);
        virtual ~Property() = default;
        Property(const Property&) = default;
        Property& operator=(const Property&) = default;
        Property(Property&&) = default;
        Property& operator=(Property&&) = default;

        [[nodiscard]] virtual std::unique_ptr<Property> clone() const = 0;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(PropertyVisitor& v) = 0;

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

    protected:
        PropertyTag tag_;

        friend class PropertyList;
    };
} // namespace SatisfactorySave
