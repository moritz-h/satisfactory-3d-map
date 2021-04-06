#ifndef SATISFACTORY3DMAP_FLOATPROPERTY_H
#define SATISFACTORY3DMAP_FLOATPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class FloatProperty : public Property {
    public:
        FloatProperty(std::string property_name, std::string property_type, std::istream& stream);

        void accept(PropertyVisitor& v) override;

        float value() const {
            return value_;
        }

    protected:
        float value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FLOATPROPERTY_H
