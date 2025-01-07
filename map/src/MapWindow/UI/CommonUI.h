#pragma once

#include <functional>
#include <string>
#include <vector>

#include "MapWindow/DataView/ObjectProxy.h"

namespace Satisfactory3DMap::UI {

    using StringCallback = std::function<void(const std::string&)>;
    using BinDataCallback = std::function<void(std::vector<char>)>;
    using ProxyCallback = std::function<void(const ObjectProxyPtr&)>;

    class EventContext {
    public:
        StringCallback selectClass{};
        StringCallback selectPath{};
        BinDataCallback showBinData{};
        ProxyCallback updateTransform{};
    };
} // namespace Satisfactory3DMap::UI
