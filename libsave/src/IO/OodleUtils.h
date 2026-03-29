#pragma once

#include <cstddef>
#include <span>

namespace SatisfactorySave {
    void checkOodleLibraryIsLoaded();

    void oodleDecompress(std::span<std::byte> dest, std::span<const std::byte> source);
} // namespace SatisfactorySave
