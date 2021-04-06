#ifndef SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
#define SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class InventoryItemStruct : public Struct {
    public:
        InventoryItemStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        int32_t unk1_;
        std::string class_name_;
        ObjectReference ref_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
