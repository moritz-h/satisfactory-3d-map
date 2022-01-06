#ifndef SATISFACTORY3DMAP_STRUCTPROPERTY_H
#define SATISFACTORY3DMAP_STRUCTPROPERTY_H

#include <vector>

#include "../Guid.h"
#include "../Structs/Struct.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class StructProperty : public Property {
    public:
        StructProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& structName() const {
            return tag_.StructName;
        }

        std::string guid() const {
            return tag_.StructGuid.toString();
        }

        const std::unique_ptr<Struct>& value() const {
            return struct_;
        }

    protected:
        std::unique_ptr<Struct> struct_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTPROPERTY_H
