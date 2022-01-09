#ifndef SATISFACTORY3DMAP_ARRAYPROPERTY_H
#define SATISFACTORY3DMAP_ARRAYPROPERTY_H

#include <vector>

#include "../Arrays/Array.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class ArrayProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::string& arrayType() const {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::unique_ptr<Array> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAYPROPERTY_H
