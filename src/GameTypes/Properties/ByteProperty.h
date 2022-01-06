#ifndef SATISFACTORY3DMAP_BYTEPROPERTY_H
#define SATISFACTORY3DMAP_BYTEPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class ByteProperty : public Property {
    public:
        ByteProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& byteType() const {
            return tag_.EnumName;
        }

        const std::string& value() const {
            return value_;
        }

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEPROPERTY_H
