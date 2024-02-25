#pragma once

#include "../Properties/Base/PropertyList.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PropertyStruct : public StructImpl<PropertyStruct, PropertyList> {
    public:
        using StructImpl<PropertyStruct, PropertyList>::StructImpl;
    };
} // namespace SatisfactorySave
