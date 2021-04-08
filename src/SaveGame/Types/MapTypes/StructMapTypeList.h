#ifndef SATISFACTORY3DMAP_STRUCTMAPTYPELIST_H
#define SATISFACTORY3DMAP_STRUCTMAPTYPELIST_H

#include <vector>

#include "../Structs/PropertyStruct.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class StructMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void parseEntry(std::istream& stream) override;

        std::size_t listSize() const override {
            return list_.size();
        }

        const std::vector<std::unique_ptr<PropertyStruct>>& list() const {
            return list_;
        }

    protected:
        std::vector<std::unique_ptr<PropertyStruct>> list_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTMAPTYPELIST_H
