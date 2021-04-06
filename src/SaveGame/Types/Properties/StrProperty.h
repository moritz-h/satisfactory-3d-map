#ifndef SATISFACTORY3DMAP_STRPROPERTY_H
#define SATISFACTORY3DMAP_STRPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class StrProperty : public Property {
    public:
        StrProperty(std::string property_name, std::string property_type, std::istream& stream);

        void accept(PropertyVisitor& v) override;

        const std::string& value() const {
            return value_;
        }

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRPROPERTY_H
