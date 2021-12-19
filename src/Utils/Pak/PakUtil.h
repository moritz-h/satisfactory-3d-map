#ifndef SATISFACTORY3DMAP_PAKUTIL_H
#define SATISFACTORY3DMAP_PAKUTIL_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace Satisfactory3DMap {
    class PakUtil {
    public:
        PakUtil();

        [[nodiscard]] std::vector<std::string> getAllFilenames() const;

        std::vector<char> readAsset(const std::string& filename);

    private:
        struct PakEntry {
            int64_t Offset = -1;
            int64_t UncompressedSize = 0;
        };

        struct DirectoryEntry {
            std::string filename;
            int32_t entryIdx;
        };

        [[nodiscard]] PakEntry decodePakEntry(int32_t offset) const;

        std::ifstream pakFile_;
        std::vector<char> EncodedPakEntries;
        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PAKUTIL_H
