#include "FilesystemUtil.h"

#include <fstream>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <windows.h>
#endif

std::vector<char> Satisfactory3DMap::readFileToMemory(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file!");
    }

    file.seekg(0, std::ios::end);
    const auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(filesize);
    file.read(buffer.data(), filesize);

    return buffer;
}

std::filesystem::path Satisfactory3DMap::getFullExePath() {
#ifdef WIN32
    std::vector<wchar_t> filename;
    DWORD length;
    do {
        filename.resize(filename.size() + 1024);
        length = GetModuleFileNameW(nullptr, filename.data(), static_cast<DWORD>(filename.size()));
    } while (length >= filename.size());
    filename.resize(length);
    return std::filesystem::path(std::wstring(filename.begin(), filename.end()));
#else
    std::filesystem::path p("/proc/self/exe");
    if (!std::filesystem::exists(p) || !std::filesystem::is_symlink(p)) {
        throw std::runtime_error("Cannot read process name!");
    }
    return std::filesystem::read_symlink(p);
#endif
}

std::filesystem::path Satisfactory3DMap::getExeDir() {
    static std::filesystem::path exeDir;
    static bool exeDirCached = false;
    if (!exeDirCached) {
        try {
            exeDir = getFullExePath().parent_path();
        } catch (...) {}
        // Fallback to working directory.
        exeDir = std::filesystem::current_path();
        exeDirCached = true;
    }
    return exeDir;
}

std::filesystem::path Satisfactory3DMap::getConfigFile() {
    constexpr std::string_view filename = "Satisfactory3DMap.cfg";
    return getExeDir() / filename;
}

std::filesystem::path Satisfactory3DMap::getLogFile() {
    constexpr std::string_view filename = "Satisfactory3DMap.log";
    return getExeDir() / filename;
}

std::optional<std::filesystem::path> Satisfactory3DMap::findSteamExe() {
#ifdef WIN32
    std::size_t bufSize = 2048;
    std::wstring valueBuf(bufSize, 0);
    auto pcbData = static_cast<DWORD>(bufSize * sizeof(wchar_t));
    auto status = RegGetValueW(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"SteamExe", RRF_RT_REG_SZ, nullptr,
        static_cast<void*>(valueBuf.data()), &pcbData);
    if (status != ERROR_SUCCESS) {
        return std::nullopt;
    }

    const std::filesystem::path steamExePath(valueBuf);
    if (!std::filesystem::is_regular_file(steamExePath)) {
        return std::nullopt;
    }

    return std::filesystem::canonical(steamExePath);
#else
    return std::nullopt;
#endif
}

std::vector<std::filesystem::path> Satisfactory3DMap::findSteamDirs() {
    const auto steamExeOpt = findSteamExe();
    if (!steamExeOpt.has_value()) {
        return {};
    }

    const auto libraryVdf = steamExeOpt.value().parent_path() / "steamapps" / "libraryfolders.vdf";
    if (!std::filesystem::is_regular_file(libraryVdf)) {
        return {};
    }

    // TODO parse libraryVdf for all Steam dirs
    // TODO find appmanifest_526870.acf and read correct path

    // TODO for now just try if the default folder exists in main Steam directory

    const auto gameDir = steamExeOpt.value().parent_path() / "steamapps" / "common" / "Satisfactory";
    if (!std::filesystem::is_directory(gameDir)) {
        return {};
    }

    return {std::filesystem::canonical(gameDir)};
}

std::vector<std::filesystem::path> Satisfactory3DMap::findGameDirs() {
    // TODO find Epic dirs!
    return findSteamDirs();
}
