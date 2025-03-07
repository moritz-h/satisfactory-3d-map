#pragma once

#include <array>
#include <sstream>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // https://github.com/EpicGames/UnrealEngine/blob/5.1.1-release/Engine/Source/Runtime/Core/Public/Misc/SecureHash.h#L132-L188
    class SATISFACTORYSAVE_API FMD5Hash {
    public:
        FMD5Hash() : bIsValid(false), Bytes{} {}

        void serialize(Archive& ar) {
            ar << bIsValid;
            if (bIsValid) {
                ar.serializeRaw(Bytes.data(), Bytes.size() * sizeof(uint8_t));
            }
        }

        [[nodiscard]] std::string toString() const {
            if (!bIsValid) {
                return "";
            }
            std::stringstream ss;
            ss << std::hex;
            for (const auto& byte : Bytes) {
                ss << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
            }
            return ss.str();
        }

    private:
        bool bIsValid;
        std::array<uint8_t, 16> Bytes;
    };

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/SecureHash.h#L198-L243
    class SATISFACTORYSAVE_API FSHAHash {
    public:
        FSHAHash() : Hash{} {}

        void serialize(Archive& ar) {
            ar.serializeRaw(Hash.data(), Hash.size() * sizeof(uint8_t));
        }

    private:
        std::array<uint8_t, 20> Hash;
    };
} // namespace SatisfactorySave
