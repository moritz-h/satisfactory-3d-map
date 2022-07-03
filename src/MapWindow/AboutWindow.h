#pragma once

#include <map>
#include <string>

namespace Satisfactory3DMap {

    class AboutWindow {
    public:
        explicit AboutWindow();
        ~AboutWindow() = default;

        void renderGui();

        void show() {
            show_ = true;
        }

    protected:
        bool show_;
        std::map<std::string, std::string> libraryLicenseMap_;
    };
} // namespace Satisfactory3DMap
