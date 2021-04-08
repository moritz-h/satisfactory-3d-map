#ifndef SATISFACTORY3DMAP_MAPTYPELIST_H
#define SATISFACTORY3DMAP_MAPTYPELIST_H

#include <memory>
#include <string>
#include <utility>

namespace Satisfactory3DMap {

    class MapTypeListVisitor;

    class MapTypeList {
    public:
        MapTypeList(std::string type) : type_(std::move(type)){};
        virtual ~MapTypeList() = default;

        virtual void accept(MapTypeListVisitor& v) = 0;

        virtual void parseEntry(std::istream& stream) = 0;

        const std::string& type() const {
            return type_;
        }

        virtual std::size_t listSize() const = 0;

    protected:
        std::string type_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPTYPELIST_H
