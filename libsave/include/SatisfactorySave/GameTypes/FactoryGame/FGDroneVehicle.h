#pragma once

#include <optional>
#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../Properties/Base/PropertyList.h"
#include "../UE/Core/UObject/NameTypes.h"
#include "FGVehicle.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FDroneAction {
        FName actionStructName;
        PropertyList action;

        void serialize(Archive& ar) {
            ar << actionStructName;
            ar << action;
        }
    };

    class SATISFACTORYSAVE_API AFGDroneVehicle : public AFGVehicle {
    public:
        std::optional<FDroneAction> mActiveAction;
        std::vector<FDroneAction> mActionQueue;

        void serialize(Archive& ar) override {
            AFGVehicle::serialize(ar);
            ar << mActiveAction;
            ar << mActionQueue;
        }
    };

} // namespace SatisfactorySave
