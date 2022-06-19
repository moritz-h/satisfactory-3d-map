#pragma once

#include <filesystem>
#include <string>

namespace Satisfactory3DMap {

    class Configuration {
    public:
        Configuration();
        ~Configuration() = default;

        void saveOnDisk() const;

        [[nodiscard]] const std::string& getImGuiIni() const {
            return imGuiIni_;
        }

        void setImGuiIni(const std::string& imGuiIni) {
            imGuiIni_ = imGuiIni;
            saveOnDisk();
        }

        [[nodiscard]] const std::filesystem::path& getGameDirectory() const {
            return gameDirectory_;
        }

        void setGameDirectory(const std::filesystem::path& gameDirectory) {
            gameDirectory_ = gameDirectory;
            saveOnDisk();
        }

    protected:
        std::string imGuiIni_;
        std::filesystem::path gameDirectory_;
    };
} // namespace Satisfactory3DMap
