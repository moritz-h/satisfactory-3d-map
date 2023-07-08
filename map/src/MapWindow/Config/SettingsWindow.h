#pragma once

#include <memory>

#include "Configuration.h"

namespace Satisfactory3DMap {

    class SettingsWindow {
    public:
        explicit SettingsWindow(std::shared_ptr<Configuration> config);
        ~SettingsWindow() = default;

        void renderGui();

        bool& show() {
            return show_;
        }

    protected:
        std::shared_ptr<Configuration> config_;
        bool show_;
    };
} // namespace Satisfactory3DMap
