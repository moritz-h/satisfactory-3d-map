#ifndef SATISFACTORY3DMAP_STRUCTPROPERTY_H
#define SATISFACTORY3DMAP_STRUCTPROPERTY_H

#include <vector>

#include "../Guid.h"
#include "../Structs/Struct.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class StructProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::string& structName() const {
            return tag_.StructName;
        }

        [[nodiscard]] const Guid& guid() const {
            return tag_.StructGuid;
        }

        [[nodiscard]] const std::unique_ptr<Struct>& value() const {
            return struct_;
        }

    protected:
        std::unique_ptr<Struct> struct_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTPROPERTY_H
