#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Math/Transform.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FObjectBaseSaveHeader {
        std::string ClassName;
        FObjectReferenceDisc Reference;
        uint32_t ObjectFlags = 0;

        void serialize(Archive& ar) {
            ar << ClassName;
            ar << Reference;
            if (ar.getSaveVersion() >= 49) {
                ar << ObjectFlags;
            }
        }
    };

    struct SATISFACTORYSAVE_API FObjectSaveHeader {
        FObjectBaseSaveHeader BaseHeader;
        std::string OuterPathName;

        void serialize(Archive& ar) {
            ar << BaseHeader;
            ar << OuterPathName;
        }
    };

    struct SATISFACTORYSAVE_API FActorSaveHeader {
        FObjectBaseSaveHeader ObjectHeader;
        FTransform3f Transform{{0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
        bool NeedTransform = false;
        bool WasPlacedInLevel = false;

        void serialize(Archive& ar) {
            ar << ObjectHeader;
            ar << NeedTransform;
            ar << Transform;
            ar << WasPlacedInLevel;
        }
    };
} // namespace SatisfactorySave
