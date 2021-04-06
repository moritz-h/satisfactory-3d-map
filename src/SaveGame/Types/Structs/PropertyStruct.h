#ifndef SATISFACTORY3DMAP_PROPERTYSTRUCT_H
#define SATISFACTORY3DMAP_PROPERTYSTRUCT_H

#include <memory>
#include <vector>

#include "../Properties/Property.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class PropertyStruct : public Struct {
    public:
        PropertyStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        std::vector<std::unique_ptr<Property>> properties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYSTRUCT_H
