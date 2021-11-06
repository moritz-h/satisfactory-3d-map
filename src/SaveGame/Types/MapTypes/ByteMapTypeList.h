#ifndef SATISFACTORY3DMAP_BYTEMAPTYPELIST_H
#define SATISFACTORY3DMAP_BYTEMAPTYPELIST_H

#include <vector>

#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class ByteMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void parseEntry(std::istream& stream) override;

        void serializeEntry(std::ostream& stream, std::size_t i) override;

        std::size_t listSize() const override {
            return list_.size();
        }

        const std::vector<int8_t>& list() const {
            return list_;
        }

    protected:
        std::vector<int8_t> list_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEMAPTYPELIST_H
