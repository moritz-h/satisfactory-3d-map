#ifndef SATISFACTORY3DMAP_ASSETUTIL_H
#define SATISFACTORY3DMAP_ASSETUTIL_H

#include <vector>

#include "GameTypes/Serialization/PackageFileSummary.h"

namespace Satisfactory3DMap {
    class AssetFile {
    public:
        AssetFile(std::vector<char> uassetData, std::vector<char> uexpData);

    protected:
        std::vector<char> uassetData_;
        std::vector<char> uexpData_;

        PackageFileSummary summary_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ASSETUTIL_H
