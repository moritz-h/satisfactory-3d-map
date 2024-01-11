#pragma once

#include "../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FStripDataFlags {
        enum EStrippedData : uint8_t {
            None = 0,
            Editor = 1,
            Server = 2,
            All = 0xff,
        };

        uint8_t GlobalStripFlags = 0;
        uint8_t ClassStripFlags = 0;

        void serialize(Archive& ar) {
            ar << GlobalStripFlags;
            ar << ClassStripFlags;
        }

        [[nodiscard]] bool IsEditorDataStripped() const {
            return (GlobalStripFlags & FStripDataFlags::Editor) != 0;
        }

        [[nodiscard]] bool IsClassDataStripped(uint8_t InFlags) const {
            return (ClassStripFlags & InFlags) != 0;
        }

        void validateOnlyEditorDataIsStripped() const {
            if (GlobalStripFlags != 1 || ClassStripFlags != 0) {
                throw std::runtime_error("FStripDataFlags has unimplemented flags set!");
            }
        }
    };

} // namespace SatisfactorySave
