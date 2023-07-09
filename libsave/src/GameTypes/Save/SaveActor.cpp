#include "GameTypes/Save/SaveActor.h"

#include <glm/ext/matrix_transform.hpp>

#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::SaveActor::serialize(Archive& ar) {
    SaveObjectBase::serialize(ar);
    ar << NeedTransform;
    ar << Transform;
    ar << WasPlacedInLevel;
}

void SatisfactorySave::SaveActor::serializeProperties(SatisfactorySave::Archive& ar, int32_t length) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        auto pos_before = inAr.tell();

        inAr << parent_reference_;
        auto count = inAr.read<int32_t>();
        child_references_.resize(count);
        for (int i = 0; i < count; ++i) {
            inAr << child_references_[i];
        }

        auto pos_after = inAr.tell();

        SaveObjectBase::serializeProperties(inAr, length - static_cast<int32_t>(pos_after - pos_before));
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr << parent_reference_;
        outAr.write(static_cast<int32_t>(child_references_.size()));
        for (auto& ref : child_references_) {
            outAr << ref;
        }
        SaveObjectBase::serializeProperties(outAr, 0);
    }
}
