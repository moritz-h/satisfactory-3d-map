#pragma once

#include <memory>

#include "../IOStream.h"
#include "Archive.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API OStreamArchive : public Archive {
    public:
        explicit OStreamArchive() : is_memory_stream_(true) {
            ostream_ = std::make_unique<MemoryOStream>();
        }

        explicit OStreamArchive(const std::filesystem::path& filepath) : is_memory_stream_(false) {
            ostream_ = std::make_unique<FileOStream>(filepath);
        }

        template<typename T>
        void write(T value) {
            operator<<(value);
        }

        void write_buffer(std::vector<char>& vec) {
            serialize(vec.data(), vec.size() * sizeof(char));
        }

        [[nodiscard]] bool isIArchive() const override {
            return false;
        }

        [[nodiscard]] bool isOArchive() const override {
            return true;
        }

        std::size_t tell() override {
            return ostream_->tell();
        }

        void seek(std::size_t pos) override {
            ostream_->seek(pos);
        }

        [[nodiscard]] std::span<const std::byte> buffer_view() const {
            if (is_memory_stream_) {
                return dynamic_cast<MemoryOStream&>(*ostream_).buffer_view();
            }
            throw std::runtime_error("Not a memory stream! Cannot get buffer view!");
        }

    protected:
        void serialize(void* data, std::size_t size) override;

        void serializeString(std::string& s) override;

        void serializeName(FName& n) override;

        std::unique_ptr<OStream> ostream_;
        bool is_memory_stream_;
    };
} // namespace SatisfactorySave
