#pragma once

#include <algorithm>
#include <filesystem>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

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

    static inline std::tuple<std::string, std::string> splitOnChar(const std::string& str, char c) {
        auto splitPos = str.find(c);
        if (splitPos == std::string::npos) {
            return {str, std::string()};
        }
        return {str.substr(0, splitPos), str.substr(splitPos + 1)};
    }

    SATISFACTORYSAVE_API int natCompare(const std::string& a, const std::string& b);

    SATISFACTORYSAVE_API int natCompareCaseInsensitive(const std::string& a, const std::string& b);

    static inline bool natLess(const std::string& a, const std::string& b) {
        return natCompare(a, b) < 0;
    }

    static inline bool natLessCaseInsensitive(const std::string& a, const std::string& b) {
        return natCompareCaseInsensitive(a, b) < 0;
    }

} // namespace SatisfactorySave
