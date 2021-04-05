#ifndef SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
#define SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class InventoryItemStruct : public Struct {
    public:
        InventoryItemStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            unk1_ = read<int32_t>(stream);
            class_name_ = read_length_string(stream);
            ref_ = ObjectReference(stream);
        }

    protected:
        int32_t unk1_;
        std::string class_name_;
        ObjectReference ref_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
