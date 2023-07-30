#pragma once

#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class ByteMapTypeList : public MapTypeListImpl<ByteMapTypeList, int8_t> {
    public:
        static constexpr std::string_view TypeName = "ByteProperty";
    };
} // namespace SatisfactorySave
