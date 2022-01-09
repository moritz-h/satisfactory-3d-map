#ifndef SATISFACTORY3DMAP_ASSETUTIL_H
#define SATISFACTORY3DMAP_ASSETUTIL_H

#include <vector>

namespace Satisfactory3DMap {
    class AssetFile {
    public:
        AssetFile(std::vector<char> uassetData, std::vector<char> uexpData);

    protected:
        std::vector<char> uassetData_;
        std::vector<char> uexpData_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ASSETUTIL_H
