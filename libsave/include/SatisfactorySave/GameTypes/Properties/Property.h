#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "IO/Archive/IStreamArchive.h"
#include "PropertyTag.h"

namespace Satisfactory3DMap {

    class PropertyVisitor;

    class Property {
    public:
        static std::unique_ptr<Property> create(IStreamArchive& ar, const std::string& parentClassName);

        explicit Property(PropertyTag tag);
        virtual ~Property() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(PropertyVisitor& v) = 0;

        PropertyTag& tag() {
            return tag_;
        }

        [[nodiscard]] const FName& name() const {
            return tag_.Name;
        }

        [[nodiscard]] const FName& type() const {
            return tag_.Type;
        }

        [[nodiscard]] int32_t size() const {
            return tag_.Size;
        }

        [[nodiscard]] int32_t index() const {
            return tag_.ArrayIndex;
        }

    protected:
        PropertyTag tag_;
    };
} // namespace Satisfactory3DMap
