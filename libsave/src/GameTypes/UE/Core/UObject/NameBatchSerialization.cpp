#include "GameTypes/UE/Core/UObject/NameBatchSerialization.h"

#include <array>
#include <codecvt>
#include <locale>

#include "Utils/VectorUtils.h"

namespace SatisfactorySave {
    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Core/Private/UObject/UnrealNames.cpp#L4272
    struct SATISFACTORYSAVE_API FSerializedNameHeader {
        [[nodiscard]] uint8_t IsUtf16() const {
            return Data[0] & 0x80u;
        }

        [[nodiscard]] uint32_t Len() const {
            return ((Data[0] & 0x7Fu) << 8) + Data[1];
        }

        [[nodiscard]] uint32_t NumBytes() const {
            return IsUtf16() ? sizeof(uint16_t) * Len() : sizeof(char) * Len();
        }

        std::array<uint8_t, 2> Data{};
    };

    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Core/Private/UObject/UnrealNames.cpp#L4791
    struct SATISFACTORYSAVE_API FNameBatchLoader {
        std::vector<uint64_t> SavedHashes;
        std::vector<FSerializedNameHeader> Headers;
        std::vector<uint8_t> Strings;

        void serialize(Archive& ar) {
            if (!ar.isIArchive()) {
                throw std::runtime_error("Only IStreamArchive support implemented!");
            }

            uint32_t Num = 0;
            ar << Num;

            if (Num == 0) {
                return;
            }

            uint32_t NumStringBytes = 0;
            ar << NumStringBytes;

            uint64_t HashVersion = 0;
            ar << HashVersion;

            SavedHashes.resize(Num);
            ar.serializeRaw(SavedHashes.data(), vector_bin_size(SavedHashes));

            Headers.resize(Num);
            ar.serializeRaw(Headers.data(), vector_bin_size(Headers));

            Strings.resize(NumStringBytes);
            ar.serializeRaw(Strings.data(), vector_bin_size(Strings));
        }
    };
} // namespace SatisfactorySave

std::vector<std::string> SatisfactorySave::LoadNameBatch(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    FNameBatchLoader loader;
    ar << loader;

    std::vector<std::string> Out;
    Out.reserve(loader.Headers.size());
    char* strings_data = reinterpret_cast<char*>(loader.Strings.data());
    for (const auto& header : loader.Headers) {
        if (header.IsUtf16()) {
            std::u16string u16str(reinterpret_cast<const char16_t*>(strings_data), header.Len());
            Out.emplace_back(std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16str));
        } else {
            Out.emplace_back(strings_data, header.NumBytes());
        }
        strings_data += header.NumBytes();
    }
    return Out;
}
