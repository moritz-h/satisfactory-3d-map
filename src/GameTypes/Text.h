#pragma once

#include <cstdint>
#include <string>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    namespace ETextFlag {
        enum Type {
            Transient = (1 << 0),
            CultureInvariant = (1 << 1),
            ConvertedProperty = (1 << 2),
            Immutable = (1 << 3),
            InitializedFromString = (1 << 4),
        };
    }

    // FText
    class FText {
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

        void serialize(Archive& ar) {
            ar << Flags;

            int8_t HistoryType = -1; // ETextHistoryType::None;
            ar << HistoryType;

            if (HistoryType != -1) {
                throw std::runtime_error("FText: HistoryType != HistoryType::None not implemented!");
            }

            bool bHasCultureInvariantString = !TextData.empty() && IsCultureInvariant();
            ar << bHasCultureInvariantString;

            if (bHasCultureInvariantString) {
                ar << TextData; // CultureInvariantString
            }
        }

    private:
        uint32_t Flags = 0;
        std::string TextData;
    };
} // namespace Satisfactory3DMap
