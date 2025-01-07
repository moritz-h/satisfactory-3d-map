#pragma once

#include <functional>
#include <string>
#include <type_traits>
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

    // Anything that can be used to call ImGui::PushID().
    template<typename T>
    concept ImGuiIdType = std::same_as<T, int> || std::same_as<T, const char*> || std::is_pointer_v<T>;
} // namespace Satisfactory3DMap::UI
