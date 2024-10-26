#include "GameTypes/Save/SaveObject.h"

#include "GameTypes/UE/GameFramework/Actor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::SaveObject::serializeTOC(Archive& ar) {
    if (ar.isIArchive()) {
        bool isActor = false;
        ar << isActor;
        if (isActor) {
            Header.emplace<FActorSaveHeader>();
            ar << std::get<FActorSaveHeader>(Header);
        } else {
            Header.emplace<FObjectSaveHeader>();
            ar << std::get<FObjectSaveHeader>(Header);
        }
    } else {
        bool isActor = std::holds_alternative<FActorSaveHeader>(Header);
        ar << isActor;
        if (isActor) {
            ar << std::get<FActorSaveHeader>(Header);
        } else {
            ar << std::get<FObjectSaveHeader>(Header);
        }
    }
}

void SatisfactorySave::SaveObject::serializeData(Archive& ar, bool data_header) {
    if (ar.isIArchive()) {
        if (std::holds_alternative<FActorSaveHeader>(Header)) {
            Object = std::make_shared<AActor>();
        } else {
            Object = std::make_shared<UObject>();
        }
    }

    std::unique_ptr<StackGuard<int32_t>> save_version_stack_pusher;
    if (data_header) {
        ar << SaveVersion;
        ar << ShouldMigrateObjectRefsToPersistent;
        save_version_stack_pusher = ar.pushSaveVersion(SaveVersion);
    }

    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        // Required, because some properties have no type information.
        auto parent_info_stack_pusher = inAr.pushParentClassInfo(baseHeader().ClassName);

        // Check stream pos to validate parser.
        const auto length = inAr.read<int32_t>();

        auto pos_before = inAr.tell();
        inAr << *Object;
        auto pos_after = inAr.tell();

        // Read extras as binary buffer
        if (pos_after - pos_before != length) {
            ExtraProperties = inAr.read_buffer(length - (pos_after - pos_before));
        }

    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        auto pos_size = outAr.tell();
        outAr.write<int32_t>(0);

        auto pos_before = outAr.tell();
        outAr << *Object;
        if (!ExtraProperties.empty()) {
            outAr.write_buffer(ExtraProperties);
        }
        auto pos_after = outAr.tell();

        outAr.seek(pos_size);
        outAr.write(static_cast<int32_t>(pos_after - pos_before));
        outAr.seek(pos_after);
    }
}
