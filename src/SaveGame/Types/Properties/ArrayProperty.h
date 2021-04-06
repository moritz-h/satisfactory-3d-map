#ifndef SATISFACTORY3DMAP_ARRAYPROPERTY_H
#define SATISFACTORY3DMAP_ARRAYPROPERTY_H

#include <vector>

#include "../Arrays/Array.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class ArrayProperty : public Property {
    public:
        ArrayProperty(std::string property_name, std::string property_type, std::istream& stream);

        void accept(PropertyVisitor& v) override;

        const std::string& arrayType() const {
            return array_type_;
        }

        const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::string array_type_;
        std::unique_ptr<Array> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAYPROPERTY_H
