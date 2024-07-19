#include "GameTypes/Save/Blueprint.h"

#include "ChunkHelper.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "IO/MemoryStreams.h"
#include "IO/ZlibUtils.h"

SatisfactorySave::Blueprint::Blueprint(const std::filesystem::path& filepath) {
    IFStreamArchive fileAr(filepath);

    fileAr << header;

    auto file_data_blob = decompressChunks(fileAr);

    const auto file_data_blob_size = file_data_blob->size();
    IStreamArchive ar(std::make_unique<MemIStream>(std::move(file_data_blob)));

    // Validate blob size
    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != ar.read<int32_t>()) {
        throw std::runtime_error("Bad blob size!");
    }

    const auto TOC_size = ar.read<int32_t>();
    const auto TOC_pos = ar.tell();

    // Parse objects
    const auto num_objects = ar.read<int32_t>();
    saveObjects.reserve(num_objects);
    for (int32_t i = 0; i < num_objects; i++) {
        saveObjects.emplace_back(SaveObjectBase::create(ar));
    }

    if (ar.tell() - TOC_pos != TOC_size) {
        throw std::runtime_error("Invalid size of TOCBlob!");
    }

    const auto data_size = ar.read<int32_t>();
    const auto data_pos = ar.tell();

    // Parse object properties
    const auto num_object_data = ar.read<int32_t>();
    if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
        throw std::runtime_error("Bad number of object data!");
    }
    for (int32_t i = 0; i < num_object_data; i++) {
        // Check stream pos to validate parser.
        const auto length = ar.read<int32_t>();
        auto pos_before = ar.tell();
        saveObjects[i]->serializeProperties(ar, length);
        auto pos_after = ar.tell();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }

    if (ar.tell() - data_pos != data_size) {
        throw std::runtime_error("Invalid size of DataBlob!");
    }

    // Validate stream is completely read
    if (static_cast<long>(file_data_blob_size) != ar.tell()) {
        throw std::runtime_error("Error parsing blueprint: Size check after parsing failed!");
    }
}

void SatisfactorySave::Blueprint::save(const std::filesystem::path& filepath) {
    // Serialize data to blob
    OMemStreamArchive ar(std::make_unique<MemOStream>());

    // Size placeholder
    ar.write<int32_t>(0);

    // Write TOC
    auto pos_size = ar.tell();
    ar.write<int32_t>(0);

    auto pos_before = ar.tell();

    // Write objects
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
        ar.write(obj->isActor());
        ar << *obj;
    }

    auto pos_after = ar.tell();

    ar.seek(pos_size);
    ar.write(static_cast<int32_t>(pos_after - pos_before));
    ar.seek(pos_after);

    // Write Data
    auto blob_pos_size = ar.tell();
    ar.write<int32_t>(0);

    auto blob_pos_before = ar.tell();

    // Write object properties
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
        auto pos_size = ar.tell();
        ar.write<int32_t>(0);

        auto pos_before = ar.tell();
        obj->serializeProperties(ar, 0);
        auto pos_after = ar.tell();

        ar.seek(pos_size);
        ar.write(static_cast<int32_t>(pos_after - pos_before));
        ar.seek(pos_after);
    }

    auto blob_pos_after = ar.tell();

    ar.seek(blob_pos_size);
    ar.write(static_cast<int32_t>(blob_pos_after - blob_pos_before));
    ar.seek(blob_pos_after);

    // Store size
    auto blob_size = ar.tell();
    ar.seek(0);
    ar.write(static_cast<int32_t>(blob_size - sizeof(int32_t)));

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
