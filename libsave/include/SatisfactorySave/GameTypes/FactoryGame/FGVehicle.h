#pragma once

#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "../Engine/ReplicatedState.h"
#include "../GameFramework/Actor.h"
#include "../UObject/Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FVehiclePhysicsData {
    public:
        FName BoneName;
        FRigidBodyState BodyState;

        void serialize(Archive& ar) {
            ar << BoneName;
            ar << BodyState.Position;
            ar << BodyState.Quaternion;
            ar << BodyState.AngVel;
            ar << BodyState.LinVel;
            ar << BodyState.Flags;
        }
    };

    class SATISFACTORYSAVE_API AFGVehicle : public AActor {
    public:
        std::vector<FVehiclePhysicsData> mStoredPhysicsData;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mStoredPhysicsData;
        }
    };

} // namespace SatisfactorySave
