#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API IOStreamBase {
    public:
        using pos_type = std::uint64_t;
        using off_type = std::int64_t;

        IOStreamBase() = default;
        virtual ~IOStreamBase() = default;
        IOStreamBase(const IOStreamBase&) = delete;
        IOStreamBase& operator=(const IOStreamBase&) = delete;
        IOStreamBase(IOStreamBase&&) = default;
        IOStreamBase& operator=(IOStreamBase&&) = default;

        [[nodiscard]] virtual pos_type tell() const = 0;
        virtual void seek(pos_type pos) = 0;
        virtual void seek_relative(off_type off) = 0;
        [[nodiscard]] virtual pos_type size() const = 0;
    };

    class SATISFACTORYSAVE_API IStream : public IOStreamBase {
    public:
        virtual void read(std::span<std::byte> out) = 0;
    };

    class SATISFACTORYSAVE_API OStream : public IOStreamBase {
    public:
        virtual void write(std::span<const std::byte> in) = 0;
    };

    class SATISFACTORYSAVE_API MemoryIStream : public IStream {
    public:
        explicit MemoryIStream(std::span<const std::byte> buffer) : buffer_(buffer), pos_(0) {}

        [[nodiscard]] pos_type tell() const override {
            return pos_;
        }

        void seek(pos_type pos) override {
            if (pos > buffer_.size()) {
                throw std::out_of_range("MemoryIStream: Seek out of range.");
            }
            pos_ = pos;
        }

        void seek_relative(off_type off) override {
            if (off < 0 && static_cast<pos_type>(-off) > pos_) {
                throw std::out_of_range("MemoryIStream: Seek out of range.");
            }
            if (off > 0 && pos_ + off > buffer_.size()) {
                throw std::out_of_range("MemoryIStream: Seek out of range.");
            }
            pos_ += off;
        }

        [[nodiscard]] pos_type size() const override {
            return buffer_.size();
        }

        void read(std::span<std::byte> out) override {
            if (pos_ + out.size() > buffer_.size()) {
                throw std::runtime_error("MemoryIStream: Not enough data to read.");
            }
            std::memcpy(out.data(), buffer_.data() + pos_, out.size());
            pos_ += out.size();
        }

    private:
        std::span<const std::byte> buffer_;
        pos_type pos_;
    };

    class SATISFACTORYSAVE_API MemoryOStream : public OStream {
    public:
        MemoryOStream() : content_size_(0), pos_(0) {}

        [[nodiscard]] pos_type tell() const override {
            return pos_;
        }

        void seek(pos_type pos) override {
            if (pos > content_size_) {
                throw std::out_of_range("MemoryOStream: Seek out of range.");
            }
            pos_ = pos;
        }

        void seek_relative(off_type off) override {
            if (off < 0 && static_cast<pos_type>(-off) > pos_) {
                throw std::out_of_range("MemoryOStream: Seek out of range.");
            }
            if (off > 0 && pos_ + off > content_size_) {
                throw std::out_of_range("MemoryOStream: Seek out of range.");
            }
            pos_ += off;
        }

        [[nodiscard]] pos_type size() const override {
            return content_size_;
        }

        void write(std::span<const std::byte> in) override {
            const std::size_t required = pos_ + in.size();
            if (required > buffer_.size()) {
                const std::size_t grow = buffer_.size() + std::max(buffer_.size() / 2, static_cast<std::size_t>(64));
                const std::size_t new_size = std::max(required, grow);
                buffer_.resize(new_size);
            }
            std::memcpy(buffer_.data() + pos_, in.data(), in.size());
            pos_ += in.size();
            if (pos_ > content_size_) {
                content_size_ = pos_;
            }
        }

        [[nodiscard]] std::span<const std::byte> buffer_view() const {
            return {buffer_.data(), content_size_};
        }

    private:
        std::vector<std::byte> buffer_;
        pos_type content_size_;
        pos_type pos_;
    };

    class SATISFACTORYSAVE_API FileIStream : public IStream {
    public:
        explicit FileIStream(const std::filesystem::path& path) {
            file_.exceptions(std::ifstream::badbit);
            file_.open(path, std::ios::binary);
            if (!file_.is_open()) {
                throw std::runtime_error("FileIStream: Failed to open file for reading: " + path.string());
            }
            file_.seekg(0, std::ios::end);
            size_ = static_cast<pos_type>(file_.tellg());
            file_.seekg(0, std::ios::beg);
        }

        [[nodiscard]] pos_type tell() const override {
            auto pos = file_.tellg();
            if (pos == -1) {
                throw std::runtime_error("FileIStream: tellg failed (bad file state).");
            }
            return static_cast<pos_type>(pos);
        }

        void seek(pos_type pos) override {
            if (pos > size_) {
                throw std::out_of_range("FileIStream: Seek out of range.");
            }
            const auto prev_pos = file_.tellg();
            file_.seekg(static_cast<std::ifstream::pos_type>(pos));
            if (!file_) {
                // Restore previous position
                file_.clear();
                file_.seekg(prev_pos);
                throw std::runtime_error("FileIStream: seekg failed (bad file state).");
            }
        }

        void seek_relative(off_type off) override {
            const auto prev_pos = file_.tellg();
            file_.seekg(static_cast<std::ifstream::off_type>(off), std::ios_base::cur);
            if (!file_) {
                // Restore previous position
                file_.clear();
                file_.seekg(prev_pos);
                throw std::out_of_range("FileIStream: seekg failed (bad file state).");
            }
        }

        [[nodiscard]] pos_type size() const override {
            return size_;
        }

        void read(std::span<std::byte> out) override {
            const auto prev_pos = file_.tellg();
            file_.read(reinterpret_cast<char*>(out.data()), static_cast<std::streamsize>(out.size()));
            if (!file_ || static_cast<std::size_t>(file_.gcount()) != out.size()) {
                // Restore previous position
                file_.clear();
                file_.seekg(prev_pos);
                throw std::runtime_error("FileIStream: Could not read full buffer or stream error.");
            }
        }

    private:
        mutable std::ifstream file_;
        pos_type size_;
    };

    class SATISFACTORYSAVE_API FileOStream : public OStream {
    public:
        explicit FileOStream(const std::filesystem::path& path) : content_size_(0) {
            file_.exceptions(std::ofstream::badbit);
            file_.open(path, std::ios::binary | std::ios::trunc);
            if (!file_.is_open()) {
                throw std::runtime_error("FileOStream: Failed to open file for writing: " + path.string());
            }
        }

        [[nodiscard]] pos_type tell() const override {
            auto pos = file_.tellp();
            if (pos == -1) {
                throw std::runtime_error("FileOStream: tellp failed (bad file state).");
            }
            return static_cast<pos_type>(pos);
        }

        void seek(pos_type pos) override {
            if (pos > content_size_) {
                throw std::out_of_range("FileOStream: Seek out of range.");
            }
            const auto prev_pos = file_.tellp();
            file_.seekp(static_cast<std::ofstream::pos_type>(pos));
            if (!file_) {
                // Restore previous position
                file_.clear();
                file_.seekp(prev_pos);
                throw std::runtime_error("FileOStream: seekp failed (bad file state).");
            }
        }

        void seek_relative(off_type off) override {
            const auto prev_pos = file_.tellp();
            file_.seekp(static_cast<std::ofstream::off_type>(off), std::ios_base::cur);
            if (!file_) {
                // Restore previous position
                file_.clear();
                file_.seekp(prev_pos);
                throw std::out_of_range("FileOStream: seekp failed (bad file state).");
            }
        }

        [[nodiscard]] pos_type size() const override {
            return content_size_;
        }

        void write(std::span<const std::byte> in) override {
            auto prev_pos = file_.tellp();
            file_.write(reinterpret_cast<const char*>(in.data()), static_cast<std::streamsize>(in.size()));
            if (!file_) {
                // Restore previous position
                file_.clear();
                file_.seekp(prev_pos);
                throw std::runtime_error("FileOStream: write failed (bad file state).");
            }
            content_size_ = std::max(content_size_, static_cast<pos_type>(file_.tellp()));
        }

    private:
        mutable std::ofstream file_;
        pos_type content_size_;
    };
} // namespace SatisfactorySave
