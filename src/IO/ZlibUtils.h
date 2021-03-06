#pragma once

#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> zlibCompress(const std::vector<char>& buffer);

    std::vector<char> zlibUncompress(const std::vector<char>& buffer, std::size_t uncompressed_size);

    void zlibUncompress(char* dest, std::size_t destLen, const char* source, std::size_t sourceLen);

} // namespace Satisfactory3DMap
