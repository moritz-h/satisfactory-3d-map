#include "IO/Archive/OStreamArchive.h"

#include <algorithm>
#include <codecvt>
#include <locale>

#include "GameTypes/UE/Core/UObject/NameTypes.h"

void SatisfactorySave::OStreamArchive::serialize(void* data, std::size_t size) {
    ostream_->write(static_cast<char*>(data), static_cast<std::streamsize>(size));
    if (!ostream_->good()) {
        throw std::runtime_error("Error writing to stream!");
    }
}

void SatisfactorySave::OStreamArchive::serializeString(std::string& s) {
    if (s.empty()) {
        write<int32_t>(0);
        return;
    }

    // Detect ASCII.
    // Assume correctly encoded UTF-8, therefore, detect ASCII by simply checking if all first bits are zero.
    const bool is_ascii = !std::any_of(s.begin(), s.end(), [](const char& c) { return c & 0x80; });

    if (is_ascii) {
        const int32_t size = static_cast<int32_t>(s.size()) + 1;
        write(size);
        serialize(s.data(), size * sizeof(std::string::value_type));
    } else {
        std::u16string u16str = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(s);
        const int32_t size = static_cast<int32_t>(u16str.size()) + 1;
        write(-size);
        serialize(u16str.data(), size * sizeof(std::u16string ::value_type));
    }
}

void SatisfactorySave::OStreamArchive::serializeName(FName& n) {
    std::string s = n.toString();
    serializeString(s);
}
