#pragma once

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "../../IO/Archive/Archive.h"
#include "../Misc/Name.h"

namespace Satisfactory3DMap {

    class MapTypeListVisitor;

    class MapTypeList {
    public:
        explicit MapTypeList(FName type) : type_(std::move(type)){};
        virtual ~MapTypeList() = default;

        virtual void accept(MapTypeListVisitor& v) = 0;

        virtual void serializeEntry(Archive& ar, std::size_t i) = 0;

        [[nodiscard]] const FName& type() const {
            return type_;
        }

        [[nodiscard]] virtual std::size_t listSize() const = 0;

    protected:
        FName type_;
    };
} // namespace Satisfactory3DMap
