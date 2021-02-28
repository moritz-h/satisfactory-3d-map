#ifndef SATISFACTORYSAVEGAME_STREAMUTILS_H
#define SATISFACTORYSAVEGAME_STREAMUTILS_H

#include <algorithm>
#include <codecvt>
#include <istream>
#include <locale>
#include <string>
#include <vector>

namespace SatisfactorySaveGame {
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

    // https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html
    static std::string read_length_string(std::istream& stream) {
        auto size = read<int32_t>(stream);

        if (size == 0) {
            return std::string();
        }

        if (size > 0) {
            std::string str(size, '\0');
            stream.read(str.data(), size * sizeof(std::string::value_type));
            if (!stream.good()) {
                throw std::runtime_error("Error reading from stream!");
            }
            str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
            return str;
        }

        std::u16string u16str(-size, '\0');
        stream.read(reinterpret_cast<char*>(u16str.data()), -size * sizeof(std::u16string::value_type));
        if (!stream.good()) {
            throw std::runtime_error("Error reading from stream!");
        }
        u16str.erase(std::find(u16str.begin(), u16str.end(), '\0'), u16str.end());
        // https://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
        return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16str);
    }

    class MemStreambuf : public std::streambuf {
    private:
        std::unique_ptr<std::vector<char>> buf_;

    public:
        explicit MemStreambuf(std::unique_ptr<std::vector<char>> buf) : buf_(std::move(buf)) {
            setg(buf_->data(), buf_->data(), buf_->data() + buf_->size());
        }

        // https://stackoverflow.com/a/53200040
        // https://stackoverflow.com/a/29134696
        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) override {
            // support tellg()
            if (way == std::ios_base::cur && off == 0) {
                return gptr() - eback();
            }
            // TODO
            return -1;
        }

        std::streampos seekpos(std::streampos sp, std::ios_base::openmode which) override {
            // TODO
            return -1;
        }
    };

    class MemIStream : public std::istream {
    private:
        MemStreambuf memstreambuf_;

    public:
        explicit MemIStream(std::unique_ptr<std::vector<char>> buf)
            : std::istream(nullptr), memstreambuf_(std::move(buf)) {
            init(&memstreambuf_);
        }
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_STREAMUTILS_H
