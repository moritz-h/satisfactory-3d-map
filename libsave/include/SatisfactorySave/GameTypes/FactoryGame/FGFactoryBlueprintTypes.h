#pragma once

#include <cstdint>

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Math/Color.h"
#include "../UE/Core/Math/IntVector.h"
#include "../UE/CoreUObject/UObject/TopLevelAssetPath.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FGIconLibrary.h
    struct SATISFACTORYSAVE_API FPersistentGlobalIconId {
    public:
        FTopLevelAssetPath IconLibrary;
        int32_t IconID = 0;
    };

    struct SATISFACTORYSAVE_API FBlueprintRecord {
        int32_t ConfigVersion = 3; // from AFGBlueprintSubsystem::SerializeBlueprintConfig
        std::string BlueprintDescription;
        FPersistentGlobalIconId IconID;
        FLinearColor Color;

        void serialize(Archive& ar) {
            ar << ConfigVersion;
            if (ConfigVersion < 2 || ConfigVersion > 3) {
                throw std::runtime_error("Unknown ConfigVersion!");
            }
            ar << BlueprintDescription;
            ar << IconID.IconID;
            ar << Color;
            if (ConfigVersion >= 3) {
                ar << IconID.IconLibrary;
            }
        }
    };

    struct SATISFACTORYSAVE_API FBlueprintItemAmount {
        FObjectReferenceDisc ItemClass;
        int32_t Amount = 0;

        void serialize(Archive& ar) {
            ar << ItemClass;
            ar << Amount;
        }
    };

    struct SATISFACTORYSAVE_API FBlueprintHeader {
        int32_t HeaderVersion = 2; // from AFGBlueprintSubsystem::SerializeBlueprintHeader
        int32_t SaveVersion = 0;
        int32_t BuildVersion = 0;
        FIntVector Dimensions;
        std::vector<FBlueprintItemAmount> Cost;
        std::vector<FObjectReferenceDisc> RecipeRefs;

        void serialize(Archive& ar) {
            ar << HeaderVersion;
            if (HeaderVersion != 2) {
                throw std::runtime_error("Unknown HeaderVersion!");
            }
            ar << SaveVersion;
            ar << BuildVersion;
            ar << Dimensions;
            ar << Cost;
            ar << RecipeRefs;
        }
    };
} // namespace SatisfactorySave
