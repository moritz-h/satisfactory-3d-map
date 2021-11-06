#ifndef SATISFACTORY3DMAP_STREAMUTILS_H
#define SATISFACTORY3DMAP_STREAMUTILS_H

#include <istream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Satisfactory3DMap {
    template<typename T>
    static T read(std::istream& stream) {
        T value;
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (!stream.good()) {
            throw std::runtime_error("Error reading from stream!");
        }
        return value;
    }

    template<typename T>
    static std::vector<T> read_vector(std::istream& stream, std::size_t size) {
        std::vector<T> vec(size);
        stream.read(reinterpret_cast<char*>(vec.data()), size * sizeof(T));
        if (!stream.good()) {
            throw std::runtime_error("Error reading from stream!");
        }
        return vec;
    }

    template<typename T>
    static T read_assert_zero(std::istream& stream) {
        T value = read<T>(stream);
        if (value != static_cast<T>(0L)) {
            throw std::runtime_error("Values from stream is not zero!");
        }
        return value;
    }

    std::string read_length_string(std::istream& stream);

    template<typename T>
    static void write(std::ostream& stream, T value) {
        stream.write(reinterpret_cast<char*>(&value), sizeof(T));
        if (!stream.good()) {
            throw std::runtime_error("Error writing to stream!");
        }
    }

    template<typename T>
    static void write_vector(std::ostream& stream, std::vector<T> vec) {
        stream.write(reinterpret_cast<char*>(vec.data()), vec.size() * sizeof(T));
        if (!stream.good()) {
            throw std::runtime_error("Error writing to stream!");
        }
    }

    void write_length_string(std::ostream& stream, const std::string& value);

    class MemStreambuf : public std::streambuf {
    private:
        std::unique_ptr<std::vector<char>> buf_;

    public:
        explicit MemStreambuf(std::unique_ptr<std::vector<char>> buf) : buf_(std::move(buf)) {
            setg(buf_->data(), buf_->data(), buf_->data() + buf_->size());
        }

        // https://stackoverflow.com/a/53200040
        // https://stackoverflow.com/a/29134696
        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
            [[maybe_unused]] std::ios_base::openmode which) override {
            // support tellg()
            if (way == std::ios_base::cur && off == 0) {
                return gptr() - eback();
            }
            // TODO
            return -1;
        }

        std::streampos seekpos([[maybe_unused]] std::streampos sp,
            [[maybe_unused]] std::ios_base::openmode which) override {
            // TODO
            return -1;
        }
    };

    class MemIStream : public std::istream {
    private:
        MemStreambuf memstreambuf_;

    public:
        explicit MemIStream(std::unique_ptr<std::vector<char>> buf)
            : std::istream(nullptr),
              memstreambuf_(std::move(buf)) {
            init(&memstreambuf_);
        }
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STREAMUTILS_H
