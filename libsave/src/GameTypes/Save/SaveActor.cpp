#include "GameTypes/Save/SaveActor.h"

#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::SaveActor::SaveActor() : SaveObjectBase(true) {}

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
        inAr << child_references_;

        auto pos_after = inAr.tell();

        SaveObjectBase::serializeProperties(inAr, length - static_cast<int32_t>(pos_after - pos_before));
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr << parent_reference_;
        outAr << child_references_;

        SaveObjectBase::serializeProperties(outAr, 0);
    }
}
