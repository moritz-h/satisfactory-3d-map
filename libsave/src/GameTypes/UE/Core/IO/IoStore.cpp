#include "GameTypes/UE/Core/IO/IoStore.h"

#include "IO/Archive/IStreamArchive.h"
#include "Utils/VectorUtils.h"

void SatisfactorySave::FIoStoreTocHeader::serialize(Archive& ar) {
    ar << TocMagic;
    ar << Version;
    ar << Reserved0;
    ar << Reserved1;
    ar << TocHeaderSize;
    ar << TocEntryCount;
    ar << TocCompressedBlockEntryCount;
    ar << TocCompressedBlockEntrySize;
    ar << CompressionMethodNameCount;
    ar << CompressionMethodNameLength;
    ar << CompressionBlockSize;
    ar << DirectoryIndexSize;
    ar << PartitionCount;
    ar << ContainerId;
    ar << EncryptionKeyGuid;
    ar << ContainerFlags;
    ar << Reserved3;
    ar << Reserved4;
    ar << TocChunkPerfectHashSeedsCount;
    ar << PartitionSize;
    ar << TocChunksWithoutPerfectHashCount;
    ar << Reserved7;
    ar << Reserved8;
}

void SatisfactorySave::FIoStoreTocResource::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }
    auto& inAr = dynamic_cast<IStreamArchive&>(ar);

    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Core/Private/IO/IoStore.cpp#L2917

    inAr << Header;
    if (!Header.CheckMagic()) {
        throw std::runtime_error("Bad IoStore magic!");
    }
    if (Header.Version != 5) {
        throw std::runtime_error("Bad IoStore header version!");
    }
    if (Header.TocHeaderSize != sizeof(FIoStoreTocHeader)) {
        throw std::runtime_error("Bad IoStore header size!");
    }

    ChunkIds.resize(Header.TocEntryCount);
    inAr.serializeRaw(ChunkIds.data(), vector_bin_size(ChunkIds));

    ChunkOffsetLengths.resize(Header.TocEntryCount);
    inAr.serializeRaw(ChunkOffsetLengths.data(), vector_bin_size(ChunkOffsetLengths));

    ChunkPerfectHashSeeds.resize(Header.TocChunkPerfectHashSeedsCount);
    inAr.serializeRaw(ChunkPerfectHashSeeds.data(), vector_bin_size(ChunkPerfectHashSeeds));

    ChunkIndicesWithoutPerfectHash.resize(Header.TocChunksWithoutPerfectHashCount);
    inAr.serializeRaw(ChunkIndicesWithoutPerfectHash.data(), vector_bin_size(ChunkIndicesWithoutPerfectHash));

    CompressionBlocks.resize(Header.TocCompressedBlockEntryCount);
    inAr.serializeRaw(CompressionBlocks.data(), vector_bin_size(CompressionBlocks));

    CompressionMethods.emplace_back("None");
    for (uint32_t i = 0; i < Header.CompressionMethodNameCount; i++) {
        const auto name = inAr.read_buffer(Header.CompressionMethodNameLength);
        CompressionMethods.emplace_back(std::string(name.data()));
    }

    if (Header.ContainerFlags & static_cast<std::underlying_type_t<EIoContainerFlags>>(EIoContainerFlags::Signed)) {
        const auto HashSize = inAr.read<int32_t>();
        std::vector<uint8_t> TocSignature(HashSize);
        inAr.serializeRaw(TocSignature.data(), vector_bin_size(TocSignature));
        std::vector<uint8_t> BlockSignature(HashSize);
        inAr.serializeRaw(BlockSignature.data(), vector_bin_size(BlockSignature));
        ChunkBlockSignatures.resize(Header.TocCompressedBlockEntryCount);
        inAr.serializeRaw(ChunkBlockSignatures.data(), vector_bin_size(ChunkBlockSignatures));
    }

    DirectoryIndexBuffer.resize(Header.DirectoryIndexSize);
    inAr.serializeRaw(DirectoryIndexBuffer.data(), vector_bin_size(DirectoryIndexBuffer));

    ChunkMetas.resize(Header.TocEntryCount);
    inAr.serializeRaw(ChunkMetas.data(), vector_bin_size(ChunkMetas));
}