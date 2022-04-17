#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

namespace Satisfactory3DMap {
    static inline std::vector<std::string> splitPathName(const std::string& name) {
        std::vector<std::string> result;
        std::istringstream iss(name);
        std::string item;
        while (std::getline(iss, item, '/')) {
            if (!item.empty()) {
                result.emplace_back(item);
            }
        }
        return result;
    }

    static inline std::vector<std::string> splitPathName(std::filesystem::path path) {
        std::vector<std::string> segments;
        for (const auto& element : path) {
            const auto& s = element.string();
            if (!s.empty() && s != "/") {
                segments.push_back(s);
            }
        }
        return segments;
    }

    static inline bool startsWith(const std::string& str, const std::string& pattern) {
        // https://stackoverflow.com/a/40441240
        return str.rfind(pattern, 0) == 0;
    }

} // namespace Satisfactory3DMap
