#pragma once

#include <vector>

namespace SatisfactorySave {
    std::vector<char> oodleDecompress(const std::vector<char>& buffer, std::size_t decompressed_size);

    void oodleDecompress(char* dest, std::size_t destLen, const char* source, std::size_t sourceLen);
} // namespace SatisfactorySave
