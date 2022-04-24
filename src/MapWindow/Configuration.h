#pragma once

#include <filesystem>

namespace Satisfactory3DMap {

    class Configuration {
    public:
        Configuration();
        ~Configuration() = default;

        void saveOnDisk() const;

        [[nodiscard]] const std::filesystem::path& getGameDirectory() const {
            return gameDirectory_;
        }

        void setGameDirectory(const std::filesystem::path& gameDirectory) {
            gameDirectory_ = gameDirectory;
            saveOnDisk();
        }

    protected:
        std::filesystem::path gameDirectory_;
    };
} // namespace Satisfactory3DMap
