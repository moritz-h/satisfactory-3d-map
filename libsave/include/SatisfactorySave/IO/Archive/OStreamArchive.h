#pragma once

#include <fstream>
#include <memory>

#include "../MemoryStreams.h"
#include "Archive.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API OStreamArchive : public Archive {
    public:
        explicit OStreamArchive(std::unique_ptr<std::ostream> ostream) : ostream_(std::move(ostream)) {}

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
            return static_cast<std::size_t>(ostream_->tellp());
        }

        void seek(std::size_t pos) override {
            ostream_->seekp(static_cast<std::istream::pos_type>(pos));
        }

        std::ostream& rawStream() {
            return *ostream_;
        }

    protected:
        OStreamArchive() = default;

        void serialize(void* data, std::size_t size) override;

        void serializeString(std::string& s) override;

        void serializeName(FName& n) override;

        std::unique_ptr<std::ostream> ostream_;
    };

    class SATISFACTORYSAVE_API OFStreamArchive : public OStreamArchive {
    public:
        explicit OFStreamArchive(const std::filesystem::path& filepath) {
            auto file = std::make_unique<std::ofstream>(filepath, std::ios::binary);
            if (!file->is_open()) {
                throw std::runtime_error("Cannot write file!");
            }

            ostream_ = std::move(file);
        }
    };

    class SATISFACTORYSAVE_API OMemStreamArchive : public OStreamArchive {
    public:
        explicit OMemStreamArchive(std::unique_ptr<MemOStream> ostream) {
            ostream_ = std::move(ostream);
        }

        [[nodiscard]] const std::vector<char>& data() const {
            return dynamic_cast<MemOStream&>(*ostream_).data();
        }
    };
} // namespace SatisfactorySave
