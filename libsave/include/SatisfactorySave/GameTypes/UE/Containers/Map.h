#pragma once

#include <vector>

#include "../../../IO/Archive/Archive.h"

namespace SatisfactorySave {

    // TMap
    template<typename KeyType, typename ValueType>
    class TMap {
    public:
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
    };
} // namespace SatisfactorySave
