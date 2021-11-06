#ifndef SATISFACTORY3DMAP_OBJECTPROPERTY_H
#define SATISFACTORY3DMAP_OBJECTPROPERTY_H

#include "../ObjectReference.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class ObjectProperty : public Property {
    public:
        ObjectProperty(std::string property_name, std::string property_type, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const ObjectReference& value() const {
            return value_;
        }

    protected:
        ObjectReference value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTPROPERTY_H
