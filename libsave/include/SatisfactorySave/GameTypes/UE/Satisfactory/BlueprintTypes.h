#pragma once

#include <cstdint>

#include "../../../IO/Archive/Archive.h"
#include "../Math/Color.h"
#include "../Math/IntVector.h"
#include "ObjectReference.h"
#include "SatisfactorySave/GameTypes/UE/UObject/TopLevelAssetPath.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FGIconLibrary.h
    struct SATISFACTORYSAVE_API FPersistentGlobalIconId {
    public:
        FTopLevelAssetPath IconLibrary;
        int32_t IconID = 0;
    };

    // FBlueprintRecord - FGFactoryBlueprintTypes.h
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

    // FBlueprintItemAmount - FGFactoryBlueprintTypes.h
    struct SATISFACTORYSAVE_API FBlueprintItemAmount {
        FObjectReferenceDisc ItemClass;
        int32_t Amount = 0;

        void serialize(Archive& ar) {
            ar << ItemClass;
            ar << Amount;
        }
    };

    // FBlueprintHeader - FGFactoryBlueprintTypes.h
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
