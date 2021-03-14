#ifndef SATISFACTORY3DMAP_STRINGUTILS_H
#define SATISFACTORY3DMAP_STRINGUTILS_H

#include <sstream>
#include <string>
#include <vector>

namespace Satisfactory3DMap {
    std::vector<std::string> splitPathName(const std::string& name) {
        std::vector<std::string> result;
        std::istringstream iss(name);
        std::string item;
        while (std::getline(iss, item, '/')) {
            if (!item.empty()) {
                result.emplace_back(item);
            }
        }
        return result;
    }

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRINGUTILS_H
