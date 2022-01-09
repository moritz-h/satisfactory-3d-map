#ifndef SATISFACTORY3DMAP_ISTREAMARCHIVE_H
#define SATISFACTORY3DMAP_ISTREAMARCHIVE_H

#include <fstream>
#include <memory>

#include "../StreamUtils.h"
#include "Archive.h"

namespace Satisfactory3DMap {
    class IStreamArchive : public Archive {
    public:
        explicit IStreamArchive(std::unique_ptr<std::istream> istream) : istream_(std::move(istream)) {}

        template<typename T>
        inline T read() {
            T value;
            serialize(&value, sizeof(T));
            return value;
        }

        template<typename T>
        inline T read_ahead() {
            const auto pos = tell();
            T value;
            serialize(&value, sizeof(T));
            seek(pos);
            return value;
        }

        template<typename T>
        inline std::vector<T> read_vector(std::size_t size) {
            std::vector<T> vec(size);
            serialize(vec.data(), size * sizeof(T));
            return vec;
        }

        template<typename T>
        inline void read_assert_zero() {
            if (read<T>() != static_cast<T>(0L)) {
                throw std::runtime_error("Values from stream is not zero!");
            }
        }

        [[nodiscard]] bool isIArchive() const override {
            return true;
        };

        [[nodiscard]] bool isOArchive() const override {
            return false;
        };

        std::size_t tell() override {
            return static_cast<std::size_t>(istream_->tellg());
        };

        void seek(std::size_t pos) override {
            istream_->seekg(static_cast<std::istream::pos_type>(pos));
        };

        std::istream& rawStream() {
            return *istream_;
        };

    protected:
        IStreamArchive() = default;

        void serialize(void* data, std::size_t size) override {
            istream_->read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
            if (!istream_->good()) {
                throw std::runtime_error("Error reading from stream!");
            }
        }

        void serializeString(std::string& s) override {
            s = read_length_string(*istream_);
        }

        std::unique_ptr<std::istream> istream_;
    };

    class IFStreamArchive : public IStreamArchive {
    public:
        explicit IFStreamArchive(const std::filesystem::path& filepath) {
            auto file = std::make_unique<std::ifstream>(filepath, std::ios::binary);
            if (!file->is_open()) {
                throw std::runtime_error("Cannot read file!");
            }

            // File size
            file->seekg(0, std::ios::end);
            filesize_ = file->tellg();
            file->seekg(0, std::ios::beg);

            istream_ = std::move(file);
        }

        std::size_t size() const {
            return filesize_;
        }

    protected:
        std::size_t filesize_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ISTREAMARCHIVE_H
