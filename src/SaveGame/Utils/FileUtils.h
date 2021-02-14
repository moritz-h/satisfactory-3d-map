#ifndef SATISFACTORYSAVEGAME_FILEUTILS_H
#define SATISFACTORYSAVEGAME_FILEUTILS_H

#include <istream>
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

    static std::string read_length_string(std::istream& stream) {
        auto size = read<uint32_t>(stream);
        std::string str(size, '\0');
        stream.read(str.data(), size * sizeof(std::string::value_type));
        if (!stream.good()) {
            throw std::runtime_error("Error reading from stream!");
        }
        str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
        return str;
    }
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_FILEUTILS_H
