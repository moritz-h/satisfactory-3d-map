#include "ZlibUtils.h"

#include <stdexcept>

#include <zlib.h>

std::vector<char> Satisfactory3DMap::zlibCompress(const std::vector<char>& buffer) {
    uLong size = compressBound(static_cast<uLong>(buffer.size()));
    std::vector<char> buffer_compressed(static_cast<std::size_t>(size));
    int state = ::compress(reinterpret_cast<Bytef*>(buffer_compressed.data()), &size,
        reinterpret_cast<const Bytef*>(buffer.data()), static_cast<uLong>(buffer.size()));
    if (state != Z_OK) {
        throw std::runtime_error("Error compressing buffer with zlib!");
    }
    buffer_compressed.resize(static_cast<std::size_t>(size));
    return buffer_compressed;
}

std::vector<char> Satisfactory3DMap::zlibUncompress(const std::vector<char>& buffer, std::size_t uncompressed_size) {
    std::vector<char> buffer_uncompressed(uncompressed_size);
    auto size = static_cast<uLongf>(uncompressed_size);
    int state = ::uncompress(reinterpret_cast<Bytef*>(buffer_uncompressed.data()), &size,
        reinterpret_cast<const Bytef*>(buffer.data()), static_cast<uLong>(buffer.size()));
    if (state != Z_OK || size != uncompressed_size) {
        throw std::runtime_error("Error decompressing buffer with zlib!");
    }
    return buffer_uncompressed;
}
