#include "ChunkHelper.h"

#include "IO/ZlibUtils.h"

std::vector<std::byte> SatisfactorySave::decompressChunks(IStreamArchive& inAr) {
    std::vector<ChunkInfo> chunk_list;
    std::size_t total_decompressed_size = 0;
    while (inAr.tell() < inAr.size()) {
        ChunkHeader chunk_header;
        inAr << chunk_header;
        chunk_list.emplace_back(chunk_header, inAr.read_buffer(chunk_header.compressedSize()), total_decompressed_size);
        total_decompressed_size += chunk_header.uncompressedSize();
    }

    // Create a buffer with the total decompressed size.
    std::vector<std::byte> file_data_blob(total_decompressed_size);

    // Decompress in parallel.
    // OpenMP requires a signed integer type for loop.
    const int64_t size = static_cast<int64_t>(chunk_list.size());
#pragma omp parallel for
    for (int64_t i = 0; i < size; i++) {
        const ChunkInfo& chunk = chunk_list[i];
        std::byte* decompressed_buffer_ptr = file_data_blob.data() + chunk.decompressed_offset;
        zlibUncompress({decompressed_buffer_ptr, static_cast<std::size_t>(chunk.header.uncompressedSize())},
            chunk.compressed_chunk);
    }

    return file_data_blob;
}
