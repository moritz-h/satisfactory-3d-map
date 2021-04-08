#ifndef SATISFACTORY3DMAP_OBJECTMAPTYPELIST_H
#define SATISFACTORY3DMAP_OBJECTMAPTYPELIST_H

#include <vector>

#include "../ObjectReference.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class ObjectMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void parseEntry(std::istream& stream) override;

        std::size_t listSize() const override {
            return list_.size();
        }

        const std::vector<ObjectReference>& list() const {
            return list_;
        }

    protected:
        std::vector<ObjectReference> list_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTMAPTYPELIST_H
