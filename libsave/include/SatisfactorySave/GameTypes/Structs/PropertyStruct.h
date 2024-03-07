#pragma once

#include "../Properties/Base/PropertyList.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PropertyStruct final : public StructImplBase<PropertyStruct, PropertyList> {
    public:
        using StructImplBase<PropertyStruct, PropertyList>::StructImplBase;
    };
} // namespace SatisfactorySave
