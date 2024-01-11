#pragma once

#include <vector>

#include "../UE/Math/Transform.h"
#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FActorSaveHeader
    class SATISFACTORYSAVE_API SaveActor : public SaveObjectBase {
    public:
        using SaveObjectBase::SaveObjectBase;

        void serialize(Archive& ar) override;

        void serializeProperties(Archive& ar, int32_t length) override;

        [[nodiscard]] const FObjectReferenceDisc& parentReference() const {
            return parent_reference_;
        }

        [[nodiscard]] const std::vector<FObjectReferenceDisc>& childReferences() const {
            return child_references_;
        }

        // FActorSaveHeader
        FTransform3f Transform{{0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
        bool NeedTransform = false;
        bool WasPlacedInLevel = false;

    protected:
        FObjectReferenceDisc parent_reference_;
        std::vector<FObjectReferenceDisc> child_references_;
    };
} // namespace SatisfactorySave
