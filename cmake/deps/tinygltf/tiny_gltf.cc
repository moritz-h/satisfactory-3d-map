#include <cstdlib>

#include <zlib.h>

static unsigned char* custom_stbiw_zlib_compress(unsigned char* data, int data_len, int* out_len, int quality) {
    uLong size = compressBound(static_cast<uLong>(data_len));
    unsigned char* buffer = static_cast<unsigned char*>(::malloc(size));
    if (buffer == nullptr) {
        return nullptr;
    }
    int state = ::compress(reinterpret_cast<Bytef*>(buffer), &size, reinterpret_cast<const Bytef*>(data),
        static_cast<uLong>(data_len));
    if (state != Z_OK) {
        ::free(buffer);
        return nullptr;
    }
    *out_len = static_cast<int>(size);
    return buffer;
}

#define STBIW_ZLIB_COMPRESS custom_stbiw_zlib_compress

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
