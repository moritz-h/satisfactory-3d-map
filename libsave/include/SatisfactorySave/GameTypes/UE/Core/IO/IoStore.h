#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../Misc/Guid.h"
#include "../Misc/SecureHash.h"
#include "../UObject/NameTypes.h"
#include "IoChunkId.h"
#include "IoContainerId.h"
#include "IoDispatcher.h"
#include "IoOffsetLength.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FIoStoreTocHeader {
        static constexpr inline char TocMagicImg[] = "-==--==--==--==-";

        std::array<uint8_t, 16> TocMagic{};
        uint8_t Version = 0;
        uint8_t Reserved0 = 0;
        uint16_t Reserved1 = 0;
        uint32_t TocHeaderSize = 0;
        uint32_t TocEntryCount = 0;
        uint32_t TocCompressedBlockEntryCount = 0;
        uint32_t TocCompressedBlockEntrySize = 0;
        uint32_t CompressionMethodNameCount = 0;
        uint32_t CompressionMethodNameLength = 0;
        uint32_t CompressionBlockSize = 0;
        uint32_t DirectoryIndexSize = 0;
        uint32_t PartitionCount = 0;
        FIoContainerId ContainerId;
        FGuid EncryptionKeyGuid;
        /*EIoContainerFlags*/ uint8_t ContainerFlags = 0;
        uint8_t Reserved3 = 0;
        uint16_t Reserved4 = 0;
        uint32_t TocChunkPerfectHashSeedsCount = 0;
        uint64_t PartitionSize = 0;
        uint32_t TocChunksWithoutPerfectHashCount = 0;
        uint32_t Reserved7 = 0;
        std::array<uint64_t, 5> Reserved8{};

        [[nodiscard]] bool CheckMagic() const {
            return std::memcmp(TocMagic.data(), TocMagicImg, sizeof(TocMagic)) == 0;
        }

        void serialize(Archive& ar);
    };

    enum class FIoStoreTocEntryMetaFlags : uint8_t {
        None,
        Compressed = 1 << 0,
        MemoryMapped = 1 << 1,
    };

    struct SATISFACTORYSAVE_API FIoStoreTocEntryMeta {
        FIoChunkHash ChunkHash;
        /*FIoStoreTocEntryMetaFlags*/ uint8_t Flags = 0;
    };

    struct SATISFACTORYSAVE_API FIoStoreTocCompressedBlockEntry {
        std::array<uint8_t, 5 + 3 + 3 + 1> Data{};

        void serialize(Archive& ar) {
            ar << Data;
        }
    };

    struct SATISFACTORYSAVE_API FIoStoreTocResource {
        FIoStoreTocHeader Header;
        std::vector<FIoChunkId> ChunkIds;
        std::vector<FIoOffsetAndLength> ChunkOffsetLengths;
        std::vector<int32_t> ChunkPerfectHashSeeds;
        std::vector<int32_t> ChunkIndicesWithoutPerfectHash;
        std::vector<FIoStoreTocCompressedBlockEntry> CompressionBlocks;
        std::vector<FName> CompressionMethods;
        // FSHAHash SignatureHash;
        std::vector<FSHAHash> ChunkBlockSignatures;
        std::vector<FIoStoreTocEntryMeta> ChunkMetas;
        std::vector<uint8_t> DirectoryIndexBuffer;

        void serialize(Archive& ar);
    };
} // namespace SatisfactorySave
