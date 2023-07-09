#pragma once

#include <cstdint>
#include <ostream>
#include <string>

namespace SatisfactorySave {

    // FName
    class FName {
    public:
        std::string Name;
        uint32_t Number = 0;

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

    inline bool operator!=(const FName& name, const std::string& str) {
        return !(name == str);
    }

    inline std::string operator+(const std::string& str, const FName& name) {
        return str + name.toString();
    }

    inline std::ostream& operator<<(std::ostream& o, const FName& name) {
        return o << name.toString();
    }
} // namespace SatisfactorySave
