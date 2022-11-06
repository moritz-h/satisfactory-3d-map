#pragma once

#include <algorithm>
#include <filesystem>
#include <functional>
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

    static inline std::vector<std::string> splitPathName(const std::filesystem::path& path) {
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

    static inline bool endsWith(const std::string& str, const std::string& pattern) {
        return str.size() >= pattern.size() && str.compare(str.size() - pattern.size(), pattern.size(), pattern) == 0;
    }

    static inline bool contains(const std::string& str, const std::string& search) {
        const std::boyer_moore_searcher searcher(search.begin(), search.end());
        const auto it = std::search(str.begin(), str.end(), searcher);
        return it != str.end();
    }

    static inline bool containsCaseInsensitive(std::string str, std::string search) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) { return std::tolower(c); });
        return contains(str, search);
    }

} // namespace Satisfactory3DMap
