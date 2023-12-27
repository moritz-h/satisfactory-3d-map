#include "Utils/StringUtils.h"

#include <strnatcmp.h>

int SatisfactorySave::natCompare(const std::string& a, const std::string& b) {
    return strnatcmp(a.c_str(), b.c_str());
}

int SatisfactorySave::natCompareCaseInsensitive(const std::string& a, const std::string& b) {
    return strnatcasecmp(a.c_str(), b.c_str());
}
