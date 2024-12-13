#pragma once

#include <functional>
#include <string>

namespace Satisfactory3DMap::UI {

    using StringCallback = std::function<void(const std::string&)>;

    class SelectionContext {
    public:
        StringCallback classCallback{};
        StringCallback pathCallback{};
    };
} // namespace Satisfactory3DMap::UI
