#pragma once

#include "SaveObjectBase.h"

namespace SatisfactorySave {

    // FObjectSaveHeader
    class SaveObject : public SaveObjectBase {
    public:
        using SaveObjectBase::SaveObjectBase;

        void serialize(Archive& ar) override;

        [[nodiscard]] const std::string& outerPathName() const {
            return outer_path_name_;
        }

    protected:
        std::string outer_path_name_;
    };
} // namespace SatisfactorySave
