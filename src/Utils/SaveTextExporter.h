#ifndef SATISFACTORY3DMAP_SAVETEXTEXPORTER_H
#define SATISFACTORY3DMAP_SAVETEXTEXPORTER_H

#include <string>

#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {
    void saveToTextFile(const SaveGame& savegame, const std::string& filename);

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVETEXTEXPORTER_H
