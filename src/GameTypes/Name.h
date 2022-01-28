#ifndef SATISFACTORY3DMAP_NAME_H
#define SATISFACTORY3DMAP_NAME_H

#include <cstdint>
#include <string>

namespace Satisfactory3DMap {

    // FName
    struct FName {
        std::string Name;
        uint32_t Number = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_NAME_H
