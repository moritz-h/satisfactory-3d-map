#pragma once

#include <vector>

#include "../../IO/Archive/Archive.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // from Engine/Classes/GameFramework/OnlineReplStructs.h
    struct SATISFACTORYSAVE_API FUniqueNetIdRepl {
    public:
        // EUniqueIdEncodingFlags from Engine/Private/OnlineReplStructs.cpp
        // First 5 bits are type, remaining bits are flags:
        //   NotEncoded = 0
        //   IsEncoded = (1 << 0)
        //   IsEmpty = (1 << 1)
        //   IsPadded = (1 << 2)
        uint8_t EncodingFlags;
        /*UE::Online::EOnlineServices*/ uint8_t OnlineServicesType;
        std::vector<uint8_t> ReplicationData;

        void serialize(Archive& ar) {
            // TODO assumes NetSerialize
            ar << EncodingFlags;
            if (EncodingFlags & 1) {
                if (!(EncodingFlags & 1 << 1)) {
                    if (EncodingFlags >> 3 == 30) {
                        // V2
                        ar << OnlineServicesType;
                        ar << ReplicationData;
                    } else {
                        // V1Encoded
                        if (EncodingFlags >> 3 == 31) {
                            throw std::runtime_error("FUniqueNetIdRepl V1Encoded TypeString not implemented!");
                        }
                        uint8_t EncodedSize = static_cast<uint8_t>(ReplicationData.size());
                        ar << EncodedSize;
                        ReplicationData.resize(EncodedSize);
                        ar.serializeRaw(ReplicationData.data(), EncodedSize);
                    }
                }
            } else {
                // V1Unencoded
                throw std::runtime_error("FUniqueNetIdRepl V1Unencoded not implemented!");
            }
        }
    };

    class SATISFACTORYSAVE_API AFGPlayerState : public AActor {
    public:
        FUniqueNetIdRepl Id;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << Id;
        }
    };

} // namespace SatisfactorySave
