#include "ZlibUtils.h"

#include <stdexcept>

#include <zlib.h>

std::vector<std::byte> SatisfactorySave::zlibCompress(const std::vector<std::byte>& buffer) {
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

std::vector<std::byte> SatisfactorySave::zlibUncompress(const std::vector<std::byte>& buffer,
    std::size_t uncompressed_size) {
    std::vector<std::byte> buffer_uncompressed(uncompressed_size);
    zlibUncompress(buffer_uncompressed.data(), uncompressed_size, buffer.data(), buffer.size());
    return buffer_uncompressed;
}

void SatisfactorySave::zlibUncompress(std::byte* dest, std::size_t destLen, const std::byte* source,
    std::size_t sourceLen) {
    auto size = static_cast<uLongf>(destLen);
    int state = ::uncompress(reinterpret_cast<Bytef*>(dest), &size, reinterpret_cast<const Bytef*>(source),
        static_cast<uLong>(sourceLen));
    if (state != Z_OK || size != destLen) {
        throw std::runtime_error("Error decompressing buffer with zlib!");
    }
}
