#pragma once

#include "../Properties/Properties.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class PropertyStruct : public StructImpl<PropertyStruct, Properties> {
    public:
        explicit PropertyStruct(FName name) : StructImpl<PropertyStruct, Properties>::StructImpl(std::move(name)) {
            Data.setParentClass(name_.toString());
        }
    };
} // namespace SatisfactorySave
