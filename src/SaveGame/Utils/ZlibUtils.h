#ifndef SATISFACTORYSAVEGAME_ZLIBUTILS_H
#define SATISFACTORYSAVEGAME_ZLIBUTILS_H

#include <vector>

namespace SatisfactorySaveGame {
    std::vector<char> zlibCompress(const std::vector<char>& buffer);

    std::vector<char> zlibUncompress(const std::vector<char>& buffer, std::size_t uncompressed_size);

} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_ZLIBUTILS_H
