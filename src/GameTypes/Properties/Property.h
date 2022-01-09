#ifndef SATISFACTORY3DMAP_PROPERTY_H
#define SATISFACTORY3DMAP_PROPERTY_H

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
        static std::unique_ptr<Property> create(IStreamArchive& ar);

        explicit Property(PropertyTag tag);
        virtual ~Property() = default;

        virtual void serialize(Archive& ar);

        virtual void accept(PropertyVisitor& v) = 0;

        const std::string& name() const {
            return tag_.Name;
        }

        const std::string& type() const {
            return tag_.Type;
        }

        int32_t size() const {
            return tag_.Size;
        }

        int32_t index() const {
            return tag_.ArrayIndex;
        }

    protected:
        PropertyTag tag_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTY_H
