#pragma once

#include <glm/vec3.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    template<typename T>
    class MaterialInputStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override {
            // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Materials/MaterialShared.cpp#L285
            ar << OutputIndex;
            ar << InputName;
            int32_t temp = 0;
            ar << temp << temp << temp << temp << temp;
            ar << ExpressionName;
            // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Materials/MaterialShared.cpp#L341
            bool bTemp = false;
            ar << bTemp;
            T tempType;
            ar << tempType;
        }

    protected:
        int32_t OutputIndex = 0;
        FName InputName;
        FName ExpressionName;
    };

    class ScalarMaterialInputStruct : public MaterialInputStruct<float> {
    public:
        using MaterialInputStruct<float>::MaterialInputStruct;

        void accept(StructVisitor& v) override;
    };

    class VectorMaterialInputStruct : public MaterialInputStruct<glm::vec3> {
    public:
        using MaterialInputStruct<glm::vec3>::MaterialInputStruct;

        void accept(StructVisitor& v) override;
    };
} // namespace Satisfactory3DMap
