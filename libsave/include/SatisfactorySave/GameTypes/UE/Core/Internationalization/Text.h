#pragma once

#include <cstdint>
#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    namespace ETextFlag {
        enum Type {
            Transient = (1 << 0),
            CultureInvariant = (1 << 1),
            ConvertedProperty = (1 << 2),
            Immutable = (1 << 3),
            InitializedFromString = (1 << 4),
        };
    }

    class SATISFACTORYSAVE_API FText {
    public:
        [[nodiscard]] bool IsTransient() const {
            return (Flags & ETextFlag::Transient) != 0;
        }

        [[nodiscard]] bool IsCultureInvariant() const {
            return (Flags & ETextFlag::CultureInvariant) != 0;
        }

        [[nodiscard]] bool IsInitializedFromString() const {
            return (Flags & ETextFlag::InitializedFromString) != 0;
        }

        [[nodiscard]] const std::string& string() const {
            return TextData;
        }

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Internationalization/TextHistory.h#L16-L34
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Internationalization/Text.cpp#L805
        void serialize(Archive& ar) {
            ar << Flags;
            ar << HistoryType;

            if (HistoryType == -1) { // ETextHistoryType::None
                bool bHasCultureInvariantString = !TextData.empty() && IsCultureInvariant();
                ar << bHasCultureInvariantString;

                if (bHasCultureInvariantString) {
                    ar << TextData;
                }
            } else if (HistoryType == 0) { // ETextHistoryType::Base
                // FTextKey::SerializeAsString is using a custom string serialization (TextKeyUtil::SaveKeyString).
                // This code always writes a null termintor, also for empty strings. Do the same for binary
                // compatibility.
                static int32_t one = 1;
                static int8_t zero = 0;

                if (ar.isIArchive() || !Namespace.empty()) {
                    ar << Namespace;
                } else {
                    ar << one;
                    ar << zero;
                }
                if (ar.isIArchive() || !Key.empty()) {
                    ar << Key;
                } else {
                    ar << one;
                    ar << zero;
                }

                ar << SourceString;

                // Write something to TextData for UI:
                TextData = "[Namespace:]" + Namespace + "|[Key:]" + Key + "|[SourceString:]" + SourceString;
            } else {
                throw std::runtime_error(
                    "FText: ETextHistoryType != Base not implemented! " + std::to_string(HistoryType));
            }
        }

    private:
        uint32_t Flags = 0;
        std::string TextData;
        int8_t HistoryType = -1; // ETextHistoryType::None;

        // ETextHistoryType::Base values:
        std::string Namespace;
        std::string Key;
        std::string SourceString;
    };
} // namespace SatisfactorySave
