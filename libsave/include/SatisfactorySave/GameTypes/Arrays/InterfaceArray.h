#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class InterfaceArray : public ArrayImpl<InterfaceArray, FObjectReferenceDisc> {
    public:
        static constexpr std::string_view TypeName = "InterfaceProperty";
    };
} // namespace SatisfactorySave
