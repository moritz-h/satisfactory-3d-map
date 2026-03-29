#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace SatisfactorySave {
    std::vector<std::byte> zlibCompress(std::span<const std::byte> buffer);

    void zlibUncompress(std::span<std::byte> dest, std::span<const std::byte> source);
} // namespace SatisfactorySave
