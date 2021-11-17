#ifndef SATISFACTORY3DMAP_ENUMPROPERTY_H
#define SATISFACTORY3DMAP_ENUMPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class EnumProperty : public Property {
    public:
        EnumProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& enumType() const {
            return tag_.EnumName;
        }

        const std::string& value() const {
            return value_;
        }

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ENUMPROPERTY_H
