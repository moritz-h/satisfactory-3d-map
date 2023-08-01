#pragma once

#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FObjectSaveHeader
    class SaveObject : public SaveObjectBase {
    public:
        using SaveObjectBase::SaveObjectBase;

        void serialize(Archive& ar) override;

        // FObjectSaveHeader
        std::string OuterPathName;
    };
} // namespace SatisfactorySave
