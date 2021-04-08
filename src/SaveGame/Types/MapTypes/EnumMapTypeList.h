#ifndef SATISFACTORY3DMAP_ENUMMAPTYPELIST_H
#define SATISFACTORY3DMAP_ENUMMAPTYPELIST_H

#include <string>
#include <vector>

#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class EnumMapTypeList : public MapTypeList {
    public:
        using MapTypeList::MapTypeList;

        void accept(MapTypeListVisitor& v) override;

        void parseEntry(std::istream& stream) override;

        std::size_t listSize() const override {
            return list_.size();
        }

        const std::vector<std::string>& list() const {
            return list_;
        }

    protected:
        std::vector<std::string> list_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ENUMMAPTYPELIST_H
