#ifndef SATISFACTORY3DMAP_PROPERTYSTRUCT_H
#define SATISFACTORY3DMAP_PROPERTYSTRUCT_H

#include <memory>
#include <vector>

#include "../Properties/Property.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class PropertyStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const std::vector<std::unique_ptr<Property>>& properties() const {
            return properties_;
        }

    protected:
        std::vector<std::unique_ptr<Property>> properties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYSTRUCT_H
