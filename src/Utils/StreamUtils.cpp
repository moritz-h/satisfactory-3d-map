#include "StreamUtils.h"

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
