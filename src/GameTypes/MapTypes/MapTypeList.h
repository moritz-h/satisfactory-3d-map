#ifndef SATISFACTORY3DMAP_MAPTYPELIST_H
#define SATISFACTORY3DMAP_MAPTYPELIST_H

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    class MapTypeListVisitor;

    class MapTypeList {
    public:
        explicit MapTypeList(std::string type) : type_(std::move(type)){};
        virtual ~MapTypeList() = default;

        virtual void accept(MapTypeListVisitor& v) = 0;

        virtual void serializeEntry(Archive& ar, std::size_t i) = 0;

        [[nodiscard]] const std::string& type() const {
            return type_;
        }

        [[nodiscard]] virtual std::size_t listSize() const = 0;

    protected:
        std::string type_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPTYPELIST_H
