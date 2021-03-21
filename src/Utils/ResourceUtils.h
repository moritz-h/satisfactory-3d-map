#ifndef SATISFACTORY3DMAP_RESOURCEUTILS_H
#define SATISFACTORY3DMAP_RESOURCEUTILS_H

#include <string>
#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> getBinaryResource(const std::string& filename);

    std::string getStringResource(const std::string& filename);

    std::vector<unsigned char> getImageResource(const std::string& filename, int& width, int& height, int& channels);

    std::vector<unsigned char> getImageRGBAResource(const std::string& filename, int& width, int& height);

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_RESOURCEUTILS_H
