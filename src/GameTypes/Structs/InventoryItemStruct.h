#ifndef SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
#define SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class InventoryItemStruct : public Struct {
    public:
        InventoryItemStruct(std::string struct_name, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(StructVisitor& v) override;

        int32_t unk1() const {
            return unk1_;
        }

        const std::string& className() const {
            return class_name_;
        }

        const ObjectReference& ref() const {
            return ref_;
        }

    protected:
        int32_t unk1_;
        std::string class_name_;
        ObjectReference ref_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INVENTORYITEMSTRUCT_H
