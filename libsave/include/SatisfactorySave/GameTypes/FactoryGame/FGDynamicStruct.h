#pragma once

#include "../../IO/Archive/Archive.h"
#include "../Properties/Base/PropertyList.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FFGDynamicStruct {
    public:
        FObjectReferenceDisc ScriptStruct;
        PropertyList StructInstance;

        void serialize(Archive& ar) {
            if (ar.isIArchive()) {
                bool bHasValidStruct = false;
                ar << bHasValidStruct;

                if (bHasValidStruct) {
                    ar << ScriptStruct;

                    int32_t savedPayloadSize = -1;
                    ar << savedPayloadSize;

                    const auto pos_before = ar.tell();

                    // Archive has flags to switch between binary, native and tagged property serialization, see
                    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/Class.cpp#L2761
                    // We assume tagged properties here.
                    ar << StructInstance;

                    if (ar.tell() - pos_before != savedPayloadSize) {
                        throw std::runtime_error("Invalid FFGDynamicStruct!");
                    }
                }
            } else {
                bool bHasValidStruct = !ScriptStruct.PathName.empty();
                ar << bHasValidStruct;

                if (bHasValidStruct) {
                    ar << ScriptStruct;

                    const auto offsetBeforeSavedPayloadSize = ar.tell();

                    int32_t savedPayloadSize = -1;
                    ar << savedPayloadSize;

                    const auto offsetBeforeStructPayload = ar.tell();

                    ar << StructInstance;

                    savedPayloadSize = static_cast<int32_t>(ar.tell() - offsetBeforeStructPayload);
                    const auto currentArchiveOffset = ar.tell();
                    ar.seek(offsetBeforeSavedPayloadSize);
                    ar << savedPayloadSize;
                    ar.seek(currentArchiveOffset);
                }
            }
        }
    };
} // namespace SatisfactorySave
