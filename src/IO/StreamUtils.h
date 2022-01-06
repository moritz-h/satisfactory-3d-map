#ifndef SATISFACTORY3DMAP_STREAMUTILS_H
#define SATISFACTORY3DMAP_STREAMUTILS_H

#include <istream>
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

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STREAMUTILS_H
