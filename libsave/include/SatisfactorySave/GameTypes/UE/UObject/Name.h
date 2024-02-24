#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <utility>

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FName
    class SATISFACTORYSAVE_API FName {
    public:
        std::string Name;
        uint32_t Number = 0;

        FName() = default;
        explicit FName(std::string Name, uint32_t Number = 0) : Name(std::move(Name)), Number(Number) {}

        FName& operator=(const std::string& s) {
            Name = s;
            Number = 0;
            return *this;
        }

        [[nodiscard]] std::string toString() const {
            if (Number == 0) {
                return Name;
            }
            return Name + "_" + std::to_string(Number);
        }
    };

    inline bool operator==(const FName& name, const std::string_view& str) {
        return name.Name == str && name.Number == 0;
    }

    inline bool operator!=(const FName& name, const std::string_view& str) {
        return !(name == str);
    }

    inline std::string operator+(const std::string& str, const FName& name) {
        return str + name.toString();
    }

    inline std::ostream& operator<<(std::ostream& o, const FName& name) {
        return o << name.toString();
    }
} // namespace SatisfactorySave
