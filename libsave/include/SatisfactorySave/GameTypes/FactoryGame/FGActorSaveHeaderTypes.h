#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Math/Transform.h"
#include "ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FObjectBaseSaveHeader
    struct SATISFACTORYSAVE_API FObjectBaseSaveHeader {
        std::string ClassName;
        FObjectReferenceDisc Reference;

        void serialize(Archive& ar) {
            ar << ClassName;
            ar << Reference;
        }
    };

    // FObjectSaveHeader
    struct SATISFACTORYSAVE_API FObjectSaveHeader {
        FObjectBaseSaveHeader BaseHeader;
        std::string OuterPathName;

        void serialize(Archive& ar) {
            ar << BaseHeader;
            ar << OuterPathName;
        }
    };

    // FActorSaveHeader
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
