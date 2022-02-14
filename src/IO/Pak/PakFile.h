#ifndef SATISFACTORY3DMAP_PAKUTIL_H
#define SATISFACTORY3DMAP_PAKUTIL_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssetFile.h"
#include "IO/Archive/IStreamArchive.h"

namespace Satisfactory3DMap {
    class PakFile {
    public:
        explicit PakFile(const std::filesystem::path& pakPath);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const;

        [[nodiscard]] inline bool containsAssetFilename(const std::string& filename) const {
            return directoryEntries_.count(filename) > 0;
        }

        AssetFile readAsset(const std::string& filename);

        std::vector<char> readAssetFileContent(const std::string& filename);

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

        std::unique_ptr<IFStreamArchive> pakAr_;
        std::vector<char> EncodedPakEntries;
        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PAKUTIL_H
