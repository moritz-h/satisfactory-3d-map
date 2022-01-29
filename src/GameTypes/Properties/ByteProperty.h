#ifndef SATISFACTORY3DMAP_BYTEPROPERTY_H
#define SATISFACTORY3DMAP_BYTEPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class ByteProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& byteType() const {
            return tag_.EnumName;
        }

        [[nodiscard]] const std::string& value() const {
            return value_;
        }

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEPROPERTY_H
