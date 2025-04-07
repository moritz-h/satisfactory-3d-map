#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../FactoryGamePlugins/OnlineIntegration/LocalUserInfo.h"
#include "../UE/Core/Math/Color.h"
#include "../UE/Core/Math/IntVector.h"
#include "FGIconLibrary.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FBlueprintRecord {
        int32_t ConfigVersion = 4; // from AFGBlueprintSubsystem::SerializeBlueprintConfig
        std::string BlueprintDescription;
        FPersistentGlobalIconId IconID;
        FLinearColor Color;
        std::vector<FLocalUserNetIdBundle> LastEditedBy;

        void serialize(Archive& ar) {
            ar << ConfigVersion;
            if (ConfigVersion < 2 || ConfigVersion > 4) {
                throw std::runtime_error("Unknown ConfigVersion: " + std::to_string(ConfigVersion));
            }
            ar << BlueprintDescription;
            ar << IconID.IconID;
            ar << Color;
            if (ConfigVersion >= 3) {
                ar << IconID.IconLibrary;
            }
            if (ConfigVersion >= 4) {
                ar << LastEditedBy;
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
