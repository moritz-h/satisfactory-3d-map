#pragma once

#include <vector>

#include "../../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    template<typename KeyType, typename ValueType>
    class TMap {
    public:
        using key_type = KeyType;
        using value_type = ValueType;

        std::vector<KeyType> Keys;
        std::vector<ValueType> Values;

        TMap() = default;

        void serialize(Archive& ar) {
            if (Keys.size() != Values.size()) {
                throw std::runtime_error("Invalid map state!");
            }

            int32_t size = static_cast<int32_t>(Keys.size());
            ar << size;
            Keys.resize(size);
            Values.resize(size);

            for (int32_t i = 0; i < size; i++) {
                ar << Keys[i];
                ar << Values[i];
            }
        }

        [[nodiscard]] bool empty() const {
            if (Keys.empty() != Values.empty()) {
                throw std::runtime_error("Invalid map state!");
            }
            return Keys.empty();
        }

        [[nodiscard]] std::size_t size() const {
            if (Keys.size() != Values.size()) {
                throw std::runtime_error("Invalid map state!");
            }
            return Keys.size();
        }

        ValueType& operator[](const KeyType& key) {
            const auto it = std::find(Keys.begin(), Keys.end(), key);
            if (it != Keys.end()) {
                return Values[std::distance(Keys.begin(), it)];
            }
            // Create new entry
            Keys.emplace_back(key);
            Values.emplace_back();
            return Values.back();
        }
    };
} // namespace SatisfactorySave
