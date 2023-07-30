#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"

namespace SatisfactorySave {

    class MapTypeListVisitor;

    class MapTypeList {
    public:
        MapTypeList() = default;
        virtual ~MapTypeList() = default;

        virtual void serializeEntry(Archive& ar, std::size_t i) = 0;

        virtual void accept(MapTypeListVisitor& v) = 0;

        [[nodiscard]] virtual std::size_t listSize() const = 0;
    };
} // namespace SatisfactorySave
