#ifndef SATISFACTORY3DMAP_ARRAYPROPERTY_H
#define SATISFACTORY3DMAP_ARRAYPROPERTY_H

#include <vector>

#include "../Arrays/Array.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class ArrayProperty : public Property {
    public:
        ArrayProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& arrayType() const {
            return tag_.InnerType;
        }

        const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::unique_ptr<Array> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAYPROPERTY_H
