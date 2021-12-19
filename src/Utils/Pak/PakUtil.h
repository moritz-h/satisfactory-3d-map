#ifndef SATISFACTORY3DMAP_PAKUTIL_H
#define SATISFACTORY3DMAP_PAKUTIL_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace Satisfactory3DMap {
    class PakUtil {
    public:
        PakUtil();

    private:
        struct PakEntry {
            int64_t Offset = -1;
            int64_t UncompressedSize = 0;
        };

        struct DirectoryEntry {
            std::string filename;
            int32_t entryIdx;
        };

        std::vector<PakEntry> pakEntries_;
        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PAKUTIL_H
