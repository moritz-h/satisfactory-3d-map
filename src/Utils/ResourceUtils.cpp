#include "ResourceUtils.h"

#include <stdexcept>

#include <cmrc/cmrc.hpp>
#include <stb_image.h>

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

std::vector<unsigned char> Satisfactory3DMap::getImageResource(const std::string& filename, int& width, int& height,
    int& channels) {
    const auto file = Satisfactory3DMap::getBinaryResource(filename);

    int x = 0;
    int y = 0;
    int channels_in_file = 0;
    int desired_channels = (channels > 0 && channels <= 4) ? channels : 0;

    unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(file.data()), file.size(), &x,
        &y, &channels_in_file, desired_channels);
    if (data == nullptr) {
        throw std::runtime_error("Error loading image: " + filename);
    }
    width = x;
    height = y;
    channels = desired_channels == 0 ? channels_in_file : desired_channels;
    long long size = x * y * channels;
    std::vector<unsigned char> image(data, data + size);
    stbi_image_free(data);

    return image;
}

std::vector<unsigned char> Satisfactory3DMap::getImageRGBAResource(const std::string& filename, int& width,
    int& height) {
    int channels = 4;
    return getImageResource(filename, width, height, channels);
}
