#ifndef SATISFACTORYSAVEGAME_SAVEGAME_H
#define SATISFACTORYSAVEGAME_SAVEGAME_H

#include <filesystem>
#include <memory>
#include <vector>

#include "Types/SaveObject.h"

namespace SatisfactorySaveGame {

    class SaveGame {
    public:
        explicit SaveGame(const std::filesystem::path& filepath);

        void printHeader() const;

    private:
        struct SaveHeader {
            int32_t save_header_version;
            int32_t save_version;
            int32_t build_version;
            std::string map_name;
            std::string map_options;
            std::string session_name;
            int32_t play_duration;
            int64_t save_date_time;
            int8_t session_visibility;
        };

        SaveHeader header_;
        std::vector<std::shared_ptr<SaveObject>> save_objects_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEGAME_H
