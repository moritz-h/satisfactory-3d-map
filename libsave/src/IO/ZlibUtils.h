#pragma once

#include <cstddef>
#include <vector>

namespace SatisfactorySave {

    std::vector<std::byte> zlibCompress(const std::vector<std::byte>& buffer);

    std::vector<std::byte> zlibUncompress(const std::vector<std::byte>& buffer, std::size_t uncompressed_size);

    void zlibUncompress(std::byte* dest, std::size_t destLen, const std::byte* source, std::size_t sourceLen);

} // namespace SatisfactorySave
