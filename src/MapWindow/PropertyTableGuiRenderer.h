#ifndef SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H
#define SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "GameTypes/Properties/Property.h"

namespace Satisfactory3DMap {

    class PropertyTableGuiRenderer {
    public:
        PropertyTableGuiRenderer() = default;
        ~PropertyTableGuiRenderer() = default;

        void renderGui(const std::vector<std::unique_ptr<Property>>& properties,
            const std::function<void(const std::string&)>& callback);

    protected:
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYTABLEGUIRENDERER_H
