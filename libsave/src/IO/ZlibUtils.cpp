#include "ZlibUtils.h"

#include <stdexcept>

#include <zlib.h>

std::vector<std::byte> SatisfactorySave::zlibCompress(std::span<const std::byte> buffer) {
    uLong size = compressBound(static_cast<uLong>(buffer.size()));
    std::vector<std::byte> buffer_compressed(static_cast<std::size_t>(size));
    int state = ::compress(reinterpret_cast<Bytef*>(buffer_compressed.data()), &size,
        reinterpret_cast<const Bytef*>(buffer.data()), static_cast<uLong>(buffer.size()));
    if (state != Z_OK) {
        throw std::runtime_error("Error compressing buffer with zlib!");
    }
    buffer_compressed.resize(static_cast<std::size_t>(size));
    return buffer_compressed;
}

void SatisfactorySave::zlibUncompress(std::span<std::byte> dest, std::span<const std::byte> source) {
    auto size = static_cast<uLongf>(dest.size());
    int state = ::uncompress(reinterpret_cast<Bytef*>(dest.data()), &size,
        reinterpret_cast<const Bytef*>(source.data()), static_cast<uLong>(source.size()));
    if (state != Z_OK || size != dest.size()) {
        throw std::runtime_error("Error decompressing buffer with zlib!");
    }
}
