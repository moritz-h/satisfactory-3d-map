#ifndef SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H
#define SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H

#include <vector>

#include "SaveGame/Types/Properties/Property.h"

namespace Satisfactory3DMap {

    class PropertyTableGuiRenderer {
    public:
        PropertyTableGuiRenderer() = default;
        ~PropertyTableGuiRenderer() = default;

        void renderGui(const std::vector<std::unique_ptr<Property>>& properties);

    protected:
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H
