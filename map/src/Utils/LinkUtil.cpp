#include "LinkUtil.h"

#include <cstdlib>

void Satisfactory3DMap::openLink(const std::string& url) {
#ifdef _WIN32
    system(("start " + url).c_str());
#else
    system(("xdg-open " + url).c_str());
#endif
}
