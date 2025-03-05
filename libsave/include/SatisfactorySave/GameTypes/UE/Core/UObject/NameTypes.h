#pragma once

#include <cstdint>
#include <limits>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FName {
    public:
        std::string Name;
        uint32_t Number = NAME_NO_NUMBER_INTERNAL;

        FName() = default;
        explicit FName(std::string name) : Name(std::move(name)), Number(NAME_NO_NUMBER_INTERNAL) {
            auto it = Name.crbegin();
            int32_t digits = 0;
            for (; it < Name.crend() && *it >= '0' && *it <= '9'; ++it) {
                digits++;
            }
            static constexpr int32_t MaxDigitsInt32 = 10;
            if (it > Name.crbegin() && it < Name.crend() && *it == '_' && digits <= MaxDigitsInt32) {
                if (digits == 1 || *std::prev(it) != '0') {
                    auto substr_end = std::prev(it.base());
                    int64_t num = 0;
                    while (it > Name.crbegin()) {
                        --it;
                        num = 10 * num + (*it - '0');
                    }
                    if (num < std::numeric_limits<int32_t>::max()) {
                        Name.resize(std::distance(Name.cbegin(), substr_end));
                        Number = static_cast<uint32_t>(NAME_EXTERNAL_TO_INTERNAL(num));
                    }
                }
            }
        }
        FName(std::string Name, uint32_t Number) : Name(std::move(Name)), Number(Number) {}

        FName& operator=(const std::string& s) {
            *this = FName(s);
            return *this;
        }

        [[nodiscard]] inline std::string toString() const {
            if (Number == NAME_NO_NUMBER_INTERNAL) {
                return Name;
            }
            return Name + "_" + std::to_string(NAME_INTERNAL_TO_EXTERNAL(Number));
        }

        inline bool operator==(const FName& other) const {
            return Name == other.Name && Number == other.Number;
        }

        inline bool operator==(const std::string_view& other) const {
            // Avoid allocations for performance reasons (i.e., avoid constructor or toString() calls).
            if (Number == NAME_NO_NUMBER_INTERNAL) {
                return Name == other;
            }
            // If number is set, other needs at least two more characters for equality (underscore + one digit).
            if (other.size() < Name.size() + 2 || !other.starts_with(Name) || other[Name.size()] != '_') {
                return false;
            }
            const auto other_num_begin = Name.size() + 1;
            const std::string_view other_num(other.data() + other_num_begin, other.size() - other_num_begin);
            return other_num == std::to_string(NAME_INTERNAL_TO_EXTERNAL(Number));
        }

    private:
        static constexpr inline uint32_t NAME_NO_NUMBER_INTERNAL = 0;

        template<typename T>
        static constexpr inline T NAME_INTERNAL_TO_EXTERNAL(T x) {
            return x - 1;
        }

        template<typename T>
        static constexpr inline T NAME_EXTERNAL_TO_INTERNAL(T x) {
            return x + 1;
        }
    };

    inline std::string operator+(const std::string& str, const FName& name) {
        return str + name.toString();
    }

    inline std::ostream& operator<<(std::ostream& o, const FName& name) {
        return o << name.toString();
    }
} // namespace SatisfactorySave
