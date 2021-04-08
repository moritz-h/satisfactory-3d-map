#ifndef SATISFACTORY3DMAP_INTMAPTYPELIST_H
#define SATISFACTORY3DMAP_INTMAPTYPELIST_H

#include <vector>

#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class IntMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void parseEntry(std::istream& stream) override;

        std::size_t listSize() const override {
            return list_.size();
        }

        const std::vector<int32_t>& list() const {
            return list_;
        }

    protected:
        std::vector<int32_t> list_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INTMAPTYPELIST_H
