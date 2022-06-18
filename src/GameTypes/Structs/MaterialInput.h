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
            ar << OutputIndex_;
            ar << InputName_;
            int32_t temp = 0;
            ar << temp << temp << temp << temp << temp;
            ar << ExpressionName_;
            // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Materials/MaterialShared.cpp#L341
            bool bTemp = false;
            ar << bTemp;
            T tempType;
            ar << tempType;
        }

        [[nodiscard]] int32_t OutputIndex() const {
            return OutputIndex_;
        }

        [[nodiscard]] const FName& InputName() const {
            return InputName_;
        }

        [[nodiscard]] const FName& ExpressionName() const {
            return ExpressionName_;
        }

    protected:
        int32_t OutputIndex_ = 0;
        FName InputName_;
        FName ExpressionName_;
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
