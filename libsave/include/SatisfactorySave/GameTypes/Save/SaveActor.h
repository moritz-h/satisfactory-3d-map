#pragma once

#include <vector>

#include "../UE/Math/Transform.h"
#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FActorSaveHeader
    class SaveActor : public SaveObjectBase {
    public:
        using SaveObjectBase::SaveObjectBase;

        void serialize(Archive& ar) override;

        void serializeProperties(Archive& ar, int32_t length) override;

        [[nodiscard]] const ObjectReference& parentReference() const {
            return parent_reference_;
        }

        [[nodiscard]] const std::vector<ObjectReference>& childReferences() const {
            return child_references_;
        }

        // FActorSaveHeader
        FTransform3f Transform{{0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
        bool NeedTransform = false;
        bool WasPlacedInLevel = false;

    protected:
        ObjectReference parent_reference_;
        std::vector<ObjectReference> child_references_;
    };
} // namespace SatisfactorySave
