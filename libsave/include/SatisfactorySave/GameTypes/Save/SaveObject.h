#pragma once

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Satisfactory/ActorSaveHeader.h"
#include "../UE/UObject/Object.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SaveObject {
    public:
        SaveObject() = default;

        void serializeTOC(Archive& ar);
        void serializeData(Archive& ar, bool data_header = true);

        [[nodiscard]] bool isActor() const {
            return std::holds_alternative<FActorSaveHeader>(Header);
        }

        [[nodiscard]] const FObjectSaveHeader& objectHeader() const {
            return std::get<FObjectSaveHeader>(Header);
        }

        FObjectSaveHeader& objectHeader() {
            return std::get<FObjectSaveHeader>(Header);
        }

        [[nodiscard]] const FActorSaveHeader& actorHeader() const {
            return std::get<FActorSaveHeader>(Header);
        }

        FActorSaveHeader& actorHeader() {
            return std::get<FActorSaveHeader>(Header);
        }

        [[nodiscard]] const FObjectBaseSaveHeader& baseHeader() const {
            return isActor() ? actorHeader().ObjectHeader : objectHeader().BaseHeader;
        }

        FObjectBaseSaveHeader& baseHeader() {
            return isActor() ? actorHeader().ObjectHeader : objectHeader().BaseHeader;
        }

        std::variant<std::monostate, FActorSaveHeader, FObjectSaveHeader> Header;
        int32_t SaveVersion = 0;
        bool ShouldMigrateObjectRefsToPersistent = false;
        std::shared_ptr<UObject> Object;
        std::vector<char> ExtraProperties;
    };

    typedef std::shared_ptr<SaveObject> SaveObjectPtr;
    typedef std::vector<SaveObjectPtr> SaveObjectList;
} // namespace SatisfactorySave
