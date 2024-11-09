#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "../../CoreUObject/UObject/Object.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UActorComponent : public UObject {
    public:
        void serialize(Archive& ar) override {
            UObject::serialize(ar);

            // TArray<FSimpleMemberReference> UCSModifiedProperties;
            int32_t UCSModifiedPropertiesNum = 0;
            ar << UCSModifiedPropertiesNum;
            if (ar.isIArchive() && UCSModifiedPropertiesNum > 0) {
                throw std::runtime_error("UActorComponent: UCSModifiedProperties != 0 not implemented!");
            }
        }
    };

} // namespace SatisfactorySave
