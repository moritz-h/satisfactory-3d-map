#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "GameTypes/Properties/Properties.h"

namespace Satisfactory3DMap {

    class PropertyTableGuiRenderer {
    public:
        PropertyTableGuiRenderer() = default;
        ~PropertyTableGuiRenderer() = default;

        void renderGui(const Properties& properties, const std::function<void(const std::string&)>& callback);

    protected:
    };
} // namespace Satisfactory3DMap
