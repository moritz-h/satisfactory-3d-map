#ifndef SATISFACTORYSAVEGAME_CHUNKHEADER_H
#define SATISFACTORYSAVEGAME_CHUNKHEADER_H

#include <cstdint>
#include <istream>
#include <memory>

namespace SatisfactorySaveGame {

    class ChunkHeader {
    public:
        explicit ChunkHeader(std::istream& stream);

        [[nodiscard]] uint64_t compressedLength() const {
            return compressed_length_1_;
        }

        [[nodiscard]] uint64_t decompressedLength() const {
            return decompressed_length_1_;
        }

    protected:
        uint64_t package_file_tag_;
        uint64_t max_chunk_size_;
        uint64_t compressed_length_1_;
        uint64_t decompressed_length_1_;
        uint64_t compressed_length_2_;
        uint64_t decompressed_length_2_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_CHUNKHEADER_H
