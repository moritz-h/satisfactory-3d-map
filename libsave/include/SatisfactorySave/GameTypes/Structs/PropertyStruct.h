#pragma once

#include "../Properties/Base/PropertyList.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PropertyStruct : public StructImpl<PropertyStruct, PropertyList> {
    public:
        explicit PropertyStruct(FName name) : StructImpl<PropertyStruct, PropertyList>::StructImpl(std::move(name)) {
            Data.setParentClass(name_.toString());
        }
    };
} // namespace SatisfactorySave
