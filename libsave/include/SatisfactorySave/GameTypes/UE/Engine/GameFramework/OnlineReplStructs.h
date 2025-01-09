#pragma once

#include <type_traits>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FUniqueNetIdRepl {
    public:
        // EUniqueIdEncodingFlags from Engine/Private/OnlineReplStructs.cpp
        // First 5 bits are type, remaining bits are flags:
        enum class EUniqueIdEncodingFlags : uint8_t {
            NotEncoded = 0,
            IsEncoded = (1 << 0),
            IsEmpty = (1 << 1),
            IsPadded = (1 << 2),
            Reserved1 = (1 << 3),
            Reserved2 = (1 << 4),
            Reserved3 = (1 << 5),
            Reserved4 = (1 << 6),
            Reserved5 = (1 << 7),
            FlagsMask = (Reserved1 - 1),
            TypeMask = (0xFF ^ FlagsMask),
        };

        /*EUniqueIdEncodingFlags*/ uint8_t EncodingFlags = 0;
        /*UE::Online::EOnlineServices*/ uint8_t OnlineServicesType = 0;
        std::vector<uint8_t> ReplicationData;
        std::string TypeString;
        std::string Contents;

        void serialize(Archive& ar) {
            // TODO assumes NetSerialize
            ar << EncodingFlags;
            if (EnumHasAllFlags(EncodingFlags, EUniqueIdEncodingFlags::IsEncoded)) {
                if (!EnumHasAllFlags(EncodingFlags, EUniqueIdEncodingFlags::IsEmpty)) {
                    const uint8_t TypeHash = GetTypeHashFromEncoding(EncodingFlags);
                    if (TypeHash == TypeHash_V2) {
                        // V2
                        ar << OnlineServicesType;
                        ar << ReplicationData;
                    } else {
                        // V1Encoded
                        if (TypeHash == TypeHash_Other) {
                            ar << TypeString;
                        }
                        if (TypeHash != 0) {
                            uint8_t EncodedSize = static_cast<uint8_t>(ReplicationData.size());
                            ar << EncodedSize;
                            ReplicationData.resize(EncodedSize);
                            ar.serializeRaw(ReplicationData.data(), EncodedSize);
                        }
                    }
                }
            } else {
                // V1Unencoded
                const uint8_t TypeHash = GetTypeHashFromEncoding(EncodingFlags);
                if (TypeHash == TypeHash_Other) {
                    ar << TypeString;
                }
                if (TypeHash != 0) {
                    ar << Contents;
                }
            }
        }

    private:
        constexpr static inline uint8_t TypeHash_Other = 31;
        constexpr static inline uint8_t TypeHash_V2 = 30;

        template<typename Enum>
        constexpr static bool EnumHasAllFlags(uint8_t Flags, Enum Contains) {
            using T = std::underlying_type_t<Enum>;
            return (Flags & static_cast<T>(Contains)) == static_cast<T>(Contains);
        }

        inline static uint8_t GetTypeHashFromEncoding(uint8_t inFlags) {
            uint8_t TypeHash = (inFlags & static_cast<uint8_t>(EUniqueIdEncodingFlags::TypeMask)) >> 3;
            return (TypeHash < 32) ? TypeHash : 0;
        }
    };

} // namespace SatisfactorySave
