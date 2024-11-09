#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Core/Math/Vector.h"
#include "../Core/UObject/NameTypes.h"

namespace SatisfactorySave {
    template<typename T>
    struct FMaterialInput {
    public:
        int32_t OutputIndex = 0;
        FName InputName;
        FName ExpressionName;

        void serialize(Archive& ar) {
            // TODO seems outdated with UE5
            throw std::runtime_error("TODO, needs update for UE5.");
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
    };

    using FScalarMaterialInput = FMaterialInput<float>;
    using FVectorMaterialInput = FMaterialInput<FVector3f>;

} // namespace SatisfactorySave
