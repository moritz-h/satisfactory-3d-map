#include "GameTypes/Save/SaveObjectBase.h"

#include "GameTypes/Save/SaveActor.h"
#include "GameTypes/Save/SaveObject.h"
#include "IO/Archive/OStreamArchive.h"

std::shared_ptr<SatisfactorySave::SaveObjectBase> SatisfactorySave::SaveObjectBase::create(int32_t globalId,
    int32_t levelId, IStreamArchive& ar) {
    const auto type = ar.read_ahead<int32_t>();
    std::shared_ptr<SaveObjectBase> object;
    if (type == 0) { // object
        object = std::make_shared<SaveObject>(globalId, levelId);
    } else if (type == 1) { // actor
        object = std::make_shared<SaveActor>(globalId, levelId);
    } else {
        throw std::runtime_error("Unknown object type!");
    }
    ar << *object;
    return object;
}

SatisfactorySave::SaveObjectBase::SaveObjectBase(int32_t globalId, int32_t levelId)
    : globalId_(globalId),
      levelId_(levelId) {}

void SatisfactorySave::SaveObjectBase::serialize(Archive& ar) {
    ar << type_;
    ar << ClassName;
    ar << Reference;
}

void SatisfactorySave::SaveObjectBase::serializeProperties(SatisfactorySave::Archive& ar, int32_t length) {
    Properties.setParentClass(ClassName); // Required, because SetProperty has no type information.

    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        auto pos_before = inAr.tell();

        ar << Properties;

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/Obj.cpp#L1399
        ar << HasGuid;
        if (HasGuid) {
            ar << Guid;
        }

        auto pos_after = inAr.tell();

        // Read extras as binary buffer
        if (pos_after - pos_before != length) {
            ExtraProperties = inAr.read_buffer(length - (pos_after - pos_before));
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        ar << Properties;

        ar << HasGuid;
        if (HasGuid) {
            ar << Guid;
        }

        if (!ExtraProperties.empty()) {
            outAr.write_buffer(ExtraProperties);
        }
    }
}
