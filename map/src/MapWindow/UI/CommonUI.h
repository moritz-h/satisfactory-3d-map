#pragma once

#include <functional>
#include <string>

#include "MapWindow/DataView/ObjectProxy.h"

namespace Satisfactory3DMap::UI {

    using StringCallback = std::function<void(const std::string&)>;
    using ProxyCallback = std::function<void(const ObjectProxyPtr&)>;

    class EventContext {
    public:
        StringCallback selectClass{};
        StringCallback selectPath{};
        ProxyCallback updateTransform{};
    };
} // namespace Satisfactory3DMap::UI
