#ifndef SATISFACTORY3DMAP_RESOURCEUTILS_H
#define SATISFACTORY3DMAP_RESOURCEUTILS_H

#include <string>
#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> getBinaryResource(const std::string& filename);

    std::string getStringResource(const std::string& filename);

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_RESOURCEUTILS_H
