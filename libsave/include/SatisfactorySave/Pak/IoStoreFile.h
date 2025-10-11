#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "../GameTypes/UE/Core/IO/IoDirectoryIndex.h"
#include "../GameTypes/UE/Core/IO/IoStore.h"
#include "../IO/Archive/IStreamArchive.h"
#include "AbstractPakFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API DirectoryIndexReader {
    public:
        explicit DirectoryIndexReader(const FIoDirectoryIndexResource& res);

        [[nodiscard]] const std::unordered_map<std::string, uint32_t>& directoryEntries() const {
            return directoryEntries_;
        }

    private:
        void parseDir(uint32_t dir_idx, const std::string& path);

        void parseFile(uint32_t file_idx, const std::string& path);

        static constexpr uint32_t none = ~static_cast<uint32_t>(0);

        const FIoDirectoryIndexResource& res_;
        std::unordered_map<std::string, uint32_t> directoryEntries_;
    };

    class SATISFACTORYSAVE_API IoStoreFile : public AbstractPakFile {
    public:
        static std::shared_ptr<IoStoreFile> create(std::shared_ptr<PakManager> pakManager,
            const std::filesystem::path& path) {
            return std::make_shared<IoStoreFile>(Private(), std::move(pakManager), path);
        }

        IoStoreFile(Private p, std::shared_ptr<PakManager> pakManager, const std::filesystem::path& path);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const override;

        [[nodiscard]] bool containsAssetFilename(const std::string& filename) const override {
            return dirIndex_ != nullptr ? dirIndex_->directoryEntries().contains(filename) : false;
        }

        std::vector<char> readAssetFileContent(const std::string& filename) override;

        std::unique_ptr<IStream> getAssetFileStream(const std::string& filename) override;

        std::vector<char> readChunkContent(std::size_t chunkIdx);

    private:
        class SATISFACTORYSAVE_API AssetFileIStream : public IStream {
        public:
            explicit AssetFileIStream(std::shared_ptr<IoStoreFile> ioStore, uint32_t chunkIdx)
                : ioStore_(std::move(ioStore)),
                  pos_(0) {
                const auto& chunk = ioStore_->utoc_.ChunkOffsetLengths[chunkIdx];
                length_ = chunk.GetLength();
                blockSize_ = ioStore_->utoc_.Header.CompressionBlockSize;
                compBlockFirstIdx_ = chunk.GetOffset() / blockSize_;
                compBlockNum_ = (length_ + blockSize_ - 1) / blockSize_;
                buffer_ = std::vector<char>(blockSize_, 0);
                currentBlock_ = -1;
            }

            [[nodiscard]] pos_type tell() const override {
                return pos_;
            }

            void seek(pos_type pos) override {
                if (pos > length_) {
                    throw std::out_of_range("AssetFileIStream: Seek out of range.");
                }
                pos_ = pos;
            }

            void seek_relative(off_type off) override {
                if (off < 0 && static_cast<pos_type>(-off) > pos_) {
                    throw std::out_of_range("AssetFileIStream: Seek out of range.");
                }
                if (off > 0 && pos_ + off > length_) {
                    throw std::out_of_range("AssetFileIStream: Seek out of range.");
                }
                pos_ += off;
            }

            [[nodiscard]] pos_type size() const override {
                return length_;
            }

            void read(std::span<std::byte> out) override {
                if (pos_ + out.size() > length_) {
                    throw std::runtime_error("AssetFileIStream: Not enough data to read.");
                }
                std::byte* dst = out.data();
                std::size_t to_read = out.size();

                while (to_read > 0) {
                    uint32_t posBlock = pos_ / blockSize_;
                    if (posBlock != currentBlock_) {
                        const uint32_t expectedSize =
                            (posBlock < compBlockNum_ - 1) ? blockSize_ : length_ - (compBlockNum_ - 1) * blockSize_;
                        ioStore_->readCompressionBlock(compBlockFirstIdx_ + posBlock, buffer_.data(), expectedSize);
                        currentBlock_ = static_cast<int32_t>(posBlock);
                    }
                    const uint32_t posInBlock = pos_ % blockSize_;
                    const std::size_t read_size = std::min(static_cast<std::size_t>(blockSize_ - posInBlock), to_read);
                    std::memcpy(dst, buffer_.data() + posInBlock, read_size);
                    dst += read_size;
                    to_read -= read_size;
                    pos_ += read_size;
                }
            }

        protected:
            std::shared_ptr<IoStoreFile> ioStore_;
            uint64_t length_;
            uint32_t blockSize_;
            uint32_t compBlockFirstIdx_;
            uint32_t compBlockNum_;
            std::vector<char> buffer_;
            int32_t currentBlock_;
            pos_type pos_;
        };

        [[nodiscard]] uint32_t getChunkIdx(const std::string& filename) const;

        void readCompressionBlock(uint64_t blockIdx, char* dst, uint32_t expectedSize);

        FIoStoreTocResource utoc_;
        std::unique_ptr<DirectoryIndexReader> dirIndex_;
        std::unique_ptr<IStreamArchive> ucasAr_;
    };
} // namespace SatisfactorySave
