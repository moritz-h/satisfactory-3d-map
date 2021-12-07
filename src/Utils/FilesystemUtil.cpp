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

std::vector<char> Satisfactory3DMap::readFileToMemory(std::filesystem::path filepath) {
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

std::filesystem::path Satisfactory3DMap::getFullExeName() {
#ifdef WIN32
    std::vector<wchar_t> filename;
    DWORD length;
    do {
        filename.resize(filename.size() + 1024);
        length = GetModuleFileNameW(NULL, filename.data(), static_cast<DWORD>(filename.size()));
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
    return getFullExeName().parent_path();
}
