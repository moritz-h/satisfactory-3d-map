#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Core/Internal/IO/IoDirectoryIndex.h

    struct SATISFACTORYSAVE_API FIoDirectoryIndexEntry {
        uint32_t Name = ~static_cast<uint32_t>(0);
        uint32_t FirstChildEntry = ~static_cast<uint32_t>(0);
        uint32_t NextSiblingEntry = ~static_cast<uint32_t>(0);
        uint32_t FirstFileEntry = ~static_cast<uint32_t>(0);

        void serialize(Archive& ar) {
            ar << Name;
            ar << FirstChildEntry;
            ar << NextSiblingEntry;
            ar << FirstFileEntry;
        }
    };

    struct SATISFACTORYSAVE_API FIoFileIndexEntry {
        uint32_t Name = ~static_cast<uint32_t>(0);
        uint32_t NextFileEntry = ~static_cast<uint32_t>(0);
        uint32_t UserData = 0;

        void serialize(Archive& ar) {
            ar << Name;
            ar << NextFileEntry;
            ar << UserData;
        }
    };

    struct SATISFACTORYSAVE_API FIoDirectoryIndexResource {
        std::string MountPoint;
        std::vector<FIoDirectoryIndexEntry> DirectoryEntries;
        std::vector<FIoFileIndexEntry> FileEntries;
        std::vector<std::string> StringTable;

        void serialize(Archive& ar) {
            ar << MountPoint;
            ar << DirectoryEntries;
            ar << FileEntries;
            ar << StringTable;
        }
    };
} // namespace SatisfactorySave
