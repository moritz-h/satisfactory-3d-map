#include "StreamUtils.h"

#include <algorithm>
#include <codecvt>
#include <locale>

// https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html
std::string Satisfactory3DMap::read_length_string(std::istream& stream) {
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

void Satisfactory3DMap::write_length_string(std::ostream& stream, const std::string& value) {
    if (value.empty()) {
        write<int32_t>(stream, 0);
        return;
    }

    // Detect ASCII.
    // Assume correctly encoded UTF-8, therefore, detect ASCII by simply checking if all first bits are zero.
    bool is_ascii = !std::any_of(value.begin(), value.end(), [](const char& c) { return c & 0x80; });

    if (is_ascii) {
        int32_t size = static_cast<int32_t>(value.size()) + 1;
        write(stream, size);
        stream.write(value.data(), size * sizeof(std::string::value_type));
    } else {
        std::u16string u16str = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(value);
        int32_t size = static_cast<int32_t>(u16str.size()) + 1;
        write(stream, size);
        stream.write(reinterpret_cast<char*>(u16str.data()), size * sizeof(std::u16string ::value_type));
    }
}
