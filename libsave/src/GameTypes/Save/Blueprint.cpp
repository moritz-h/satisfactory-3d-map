#include "GameTypes/Save/Blueprint.h"

#include "ChunkHelper.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "IO/MemoryStreams.h"
#include "IO/ZlibUtils.h"
#include "SerializationUtils.h"

SatisfactorySave::Blueprint::Blueprint(const std::filesystem::path& filepath) {
    IFStreamArchive fileAr(filepath);

    fileAr << header;

    auto file_data_blob = decompressChunks(fileAr);

    const auto file_data_blob_size = file_data_blob->size();
    IStreamArchive ar(std::make_unique<MemIStream>(std::move(file_data_blob)));

    auto save_version_stack_pusher = ar.pushSaveVersion(header.SaveVersion);

    // Validate blob size
    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != ar.read<int32_t>()) {
        throw std::runtime_error("Blueprint: Bad blob size!");
    }

    parseTOCBlob<int32_t>(ar, saveObjects);
    parseDataBlob<int32_t, false>(ar, saveObjects);

    // Validate stream is completely read
    if (static_cast<long>(file_data_blob_size) != ar.tell()) {
        throw std::runtime_error("Blueprint: Size check after parsing failed!");
    }
}

void SatisfactorySave::Blueprint::save(const std::filesystem::path& filepath) {
    // Serialize data to blob
    OMemStreamArchive ar(std::make_unique<MemOStream>());

    auto save_version_stack_pusher = ar.pushSaveVersion(header.SaveVersion);

    // Size placeholder
    ar.write<int32_t>(0);

    saveTOCBlob<int32_t>(ar, saveObjects);
    saveDataBlob<int32_t, false>(ar, saveObjects);

    // Store size
    auto blob_size = ar.tell();
    ar.seek(0);
    ar.write(static_cast<int32_t>(blob_size - sizeof(int32_t)));

    save_version_stack_pusher.reset();

    // Write to file
    OFStreamArchive fileAr(filepath);

    fileAr << header;

    // Split blob into chunks
    uint64_t blob_pos = 0;
    const char* blob_buffer = ar.data().data();

    while (blob_size > 0) {
        // Compress chunk
        int64_t chunk_size = std::min(static_cast<int64_t>(blob_size), ChunkHeader::COMPRESSION_CHUNK_SIZE);
        std::vector<char> chunk_uncompressed{blob_buffer + blob_pos, blob_buffer + blob_pos + chunk_size};
        std::vector<char> chunk_compressed = zlibCompress(chunk_uncompressed);

        blob_pos += chunk_size;
        blob_size -= chunk_size;

        // Chunk header
        ChunkHeader chunkHeader(static_cast<int64_t>(chunk_compressed.size()), chunk_size);
        fileAr << chunkHeader;

        fileAr.write_buffer(chunk_compressed);
    }
}
