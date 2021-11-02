#include "FileDialogUtil.h"

#include <portable-file-dialogs.h>

std::optional<std::filesystem::path> Satisfactory3DMap::openFile() {
    auto files = pfd::open_file("Select savegame file ...", ".", {"Satisfactory Save (*.sav)", "*.sav"}).result();
    if (!files.empty()) {
        std::filesystem::path file(files[0]);
        if (std::filesystem::is_regular_file(file)) {
            return file;
        }
    }
    return std::nullopt;
}

std::optional<std::filesystem::path> Satisfactory3DMap::saveFile() {
    auto file = pfd::save_file("Select savegame file ...", ".", {"Satisfactory Save (*.sav)", "*.sav"}).result();
    if (!file.empty()) {
        return std::filesystem::path(file);
    }
    return std::nullopt;
}
