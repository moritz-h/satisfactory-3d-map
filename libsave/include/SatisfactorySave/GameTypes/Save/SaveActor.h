#pragma once

#include <vector>

#include "../UE/Math/Transform.h"
#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FActorSaveHeader
    class SATISFACTORYSAVE_API SaveActor : public SaveObjectBase {
    public:
        SaveActor();

        void serialize(Archive& ar) override;

        void serializeProperties(Archive& ar, int32_t length) override;

        // FActorSaveHeader
        FTransform3f Transform{{0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
        bool NeedTransform = false;
        bool WasPlacedInLevel = false;

        FObjectReferenceDisc parent_reference;
        std::vector<FObjectReferenceDisc> child_references;
    };
} // namespace SatisfactorySave
