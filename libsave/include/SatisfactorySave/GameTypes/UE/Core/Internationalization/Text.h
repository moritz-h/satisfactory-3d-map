#pragma once

#include <cstdint>
#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "../UObject/NameTypes.h"
#include "TextKey.h"
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
                ar << Namespace;
                ar << Key;
                ar << SourceString;

                // Write something to TextData for UI:
                TextData = "[Namespace:]" + Namespace.Str + "|[Key:]" + Key.Str + "|[SourceString:]" + SourceString;
            } else if (HistoryType == 11) { // ETextHistoryType::StringTableEntry
                ar << TableId;
                ar << Key;

                // Write something to TextData for UI:
                TextData = "[TableId:]" + TableId.toString() + "|[Key:]" + Key.Str;
            } else {
                throw std::runtime_error(
                    "FText: ETextHistoryType '" + std::to_string(HistoryType) + "' not implemented!");
            }
        }

    private:
        uint32_t Flags = 0;
        std::string TextData;
        int8_t HistoryType = -1; // ETextHistoryType::None;

        // ETextHistoryType::Base values:
        FTextKey Namespace;
        FTextKey Key;
        std::string SourceString;

        // ETextHistoryType::StringTableEntry values:
        FName TableId;
    };
} // namespace SatisfactorySave
