#pragma once

#include <cstddef>
#include <vector>

namespace SatisfactorySave {
    void checkOodleLibraryIsLoaded();

    std::vector<std::byte> oodleDecompress(const std::vector<std::byte>& buffer, std::size_t decompressed_size);

    void oodleDecompress(std::byte* dest, std::size_t destLen, const std::byte* source, std::size_t sourceLen);
} // namespace SatisfactorySave
