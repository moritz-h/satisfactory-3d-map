#include "GameTypes/Save/SaveObjectBase.h"

#include "GameTypes/Save/SaveActor.h"
#include "GameTypes/Save/SaveObject.h"
#include "IO/Archive/OStreamArchive.h"

std::shared_ptr<SatisfactorySave::SaveObjectBase> SatisfactorySave::SaveObjectBase::create(IStreamArchive& ar) {
    const auto isActor = ar.read<bool>();
    std::shared_ptr<SaveObjectBase> object;
    if (isActor) {
        object = std::make_shared<SaveActor>();
    } else {
        object = std::make_shared<SaveObject>();
    }
    ar << *object;
    return object;
}

SatisfactorySave::SaveObjectBase::SaveObjectBase(bool isActor) : isActor_(isActor) {}

void SatisfactorySave::SaveObjectBase::serialize(Archive& ar) {
    ar << ClassName;
    ar << Reference;
}

void SatisfactorySave::SaveObjectBase::serializeProperties(Archive& ar, int32_t length) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        auto pos_before = inAr.tell();

        inAr.ParentClassInfo.push(ClassName); // Required, because some properties have no type information.
        ar << Properties;
        inAr.ParentClassInfo.pop();

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
