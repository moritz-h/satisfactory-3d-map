#include "ResourceUtils.h"

#include <stdexcept>

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(resources);

std::vector<char> Satisfactory3DMap::getBinaryResource(const std::string& filename) {
    auto fs = cmrc::resources::get_filesystem();
    if (!fs.exists(filename)) {
        throw std::runtime_error("Non existing resource: " + filename);
    }
    auto file = fs.open(filename);
    return std::vector<char>(file.cbegin(), file.cend());
}

std::string Satisfactory3DMap::getStringResource(const std::string& filename) {
    auto fs = cmrc::resources::get_filesystem();
    if (!fs.exists(filename)) {
        throw std::runtime_error("Non existing resource: " + filename);
    }
    auto file = fs.open(filename);
    return std::string(file.cbegin(), file.cend());
}
