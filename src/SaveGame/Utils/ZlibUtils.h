#ifndef SATISFACTORY3DMAP_ZLIBUTILS_H
#define SATISFACTORY3DMAP_ZLIBUTILS_H

#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> zlibCompress(const std::vector<char>& buffer);

    std::vector<char> zlibUncompress(const std::vector<char>& buffer, std::size_t uncompressed_size);

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ZLIBUTILS_H
