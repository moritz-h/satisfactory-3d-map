#ifndef SATISFACTORY3DMAP_PROPERTYSTRUCT_H
#define SATISFACTORY3DMAP_PROPERTYSTRUCT_H

#include <memory>
#include <vector>

#include "../Properties/Properties.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class PropertyStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const Properties& properties() const {
            return properties_;
        }

    protected:
        Properties properties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYSTRUCT_H
