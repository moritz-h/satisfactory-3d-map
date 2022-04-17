#pragma once

#include <array>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/SecureHash.h#L198-L243
    class FSHAHash {
    public:
        FSHAHash() : Hash{} {};

        void serialize(Archive& ar) {
            ar.serializeRaw(Hash.data(), Hash.size() * sizeof(uint8_t));
        }

    private:
        std::array<uint8_t, 20> Hash;
    };
} // namespace Satisfactory3DMap
