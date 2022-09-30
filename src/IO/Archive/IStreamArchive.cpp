#include "IStreamArchive.h"

#include <algorithm>
#include <codecvt>
#include <locale>

void Satisfactory3DMap::IStreamArchive::serialize(void* data, std::size_t size) {
    validateReadLimit(size);

    istream_->read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
    if (!istream_->good()) {
        throw std::runtime_error("Error reading from stream!");
    }
}

// https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/CharacterEncoding/index.html
void Satisfactory3DMap::IStreamArchive::serializeString(std::string& s) {
    const auto size = read<int32_t>();
    validateReadLimit(size);

    if (size == 0) {
        s = std::string();
    } else if (size > 0) {
        std::string str(size, '\0');
        serialize(str.data(), size * sizeof(std::string::value_type));
        str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
        s = str;
    } else {
        std::u16string u16str(-size, '\0');
        serialize(u16str.data(), -size * sizeof(std::u16string::value_type));
        u16str.erase(std::find(u16str.begin(), u16str.end(), '\0'), u16str.end());
        // https://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
        s = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16str);
    }
}

void Satisfactory3DMap::IStreamArchive::validateReadLimit(std::size_t size) {
    if (!read_limits_.empty() && size > read_limits_.top()) {
        throw std::runtime_error("Try to read more bytes than the given read limit!");
    }
}
