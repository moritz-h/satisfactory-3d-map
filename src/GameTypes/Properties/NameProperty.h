#ifndef SATISFACTORY3DMAP_NAMEPROPERTY_H
#define SATISFACTORY3DMAP_NAMEPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class NameProperty : public Property {
    public:
        NameProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& value() const {
            return value_;
        }

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_NAMEPROPERTY_H
