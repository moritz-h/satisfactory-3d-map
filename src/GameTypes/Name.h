#ifndef SATISFACTORY3DMAP_NAME_H
#define SATISFACTORY3DMAP_NAME_H

#include <cstdint>
#include <ostream>
#include <string>

namespace Satisfactory3DMap {

    // FName
    struct FName {
        std::string Name;
        uint32_t Number = 0;

        [[nodiscard]] std::string toString() const {
            if (Number == 0) {
                return Name;
            }
            return Name + "_" + std::to_string(Number);
        }
    };

    inline bool operator==(const FName& name, const std::string& str) {
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
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_NAME_H
