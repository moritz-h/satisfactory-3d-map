#include "FileDialogUtil.h"

#include <portable-file-dialogs.h>

std::optional<std::filesystem::path> Satisfactory3DMap::openFile(std::string const& title,
    std::string const& default_path, std::vector<std::string> const& filters) {
    auto files = pfd::open_file(title, default_path, filters).result();
    if (!files.empty()) {
        std::filesystem::path file(files[0]);
        if (std::filesystem::is_regular_file(file)) {
            return file;
        }
    }
    return std::nullopt;
}

std::optional<std::filesystem::path> Satisfactory3DMap::saveFile(std::string const& title,
    std::string const& default_path, std::vector<std::string> const& filters) {
    auto file = pfd::save_file(title, default_path, filters).result();
    if (!file.empty()) {
        return std::filesystem::path(file);
    }
    return std::nullopt;
}

std::optional<std::filesystem::path> Satisfactory3DMap::selectFolder(std::string const& title,
    std::string const& default_path) {
    auto dir = pfd::select_folder(title, default_path).result();
    if (!dir.empty()) {
        std::filesystem::path dir_path(dir);
        if (std::filesystem::is_directory(dir_path)) {
            return dir_path;
        }
    }
    return std::nullopt;
}
