#pragma once

#include <string>
#include <vector>

namespace Satisfactory3DMap {
    bool resourceExists(const std::string& filename);

    std::vector<char> getBinaryResource(const std::string& filename);

    std::string getStringResource(const std::string& filename);

    std::vector<unsigned char> getImageResource(const std::string& filename, int& width, int& height, int& channels);

    std::vector<unsigned char> getImageRGBAResource(const std::string& filename, int& width, int& height);

} // namespace Satisfactory3DMap
