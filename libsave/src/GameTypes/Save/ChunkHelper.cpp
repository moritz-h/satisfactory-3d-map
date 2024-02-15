#include "ChunkHelper.h"

#include "IO/ZlibUtils.h"

std::unique_ptr<std::vector<char>> SatisfactorySave::decompressChunks(IFStreamArchive& fileAr) {
    std::vector<ChunkInfo> chunk_list;
    std::size_t total_decompressed_size = 0;
    while (fileAr.tell() < fileAr.size()) {
        ChunkHeader chunk_header;
        fileAr << chunk_header;
        chunk_list.emplace_back(chunk_header, fileAr.read_buffer(chunk_header.compressedSize()),
            total_decompressed_size);
        total_decompressed_size += chunk_header.uncompressedSize();
    }

    // Create a buffer with the total decompressed size.
    auto file_data_blob = std::make_unique<std::vector<char>>(total_decompressed_size);

    // Decompress in parallel.
    // OpenMP requires a signed integer type for loop.
    const int64_t size = static_cast<int64_t>(chunk_list.size());
#pragma omp parallel for
    for (int64_t i = 0; i < size; i++) {
        const ChunkInfo& chunk = chunk_list[i];
        char* decompressed_buffer_ptr = file_data_blob->data() + chunk.decompressed_offset;
        zlibUncompress(decompressed_buffer_ptr, chunk.header.uncompressedSize(), chunk.compressed_chunk.data(),
            chunk.compressed_chunk.size());
    }

    return file_data_blob;
}
