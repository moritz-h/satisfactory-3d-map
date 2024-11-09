#pragma once

#include "../../IO/Archive/Archive.h"
#include "FGObjectReference.h"
#include "FGVehicle.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AFGRailroadVehicle : public AFGVehicle {
    public:
        FObjectReferenceDisc mCoupledVehicleFront;
        FObjectReferenceDisc mCoupledVehicleBack;

        void serialize(Archive& ar) override {
            AFGVehicle::serialize(ar);
            ar << mCoupledVehicleFront;
            ar << mCoupledVehicleBack;
        }
    };

} // namespace SatisfactorySave
