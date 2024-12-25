#pragma once

#include "CommonUI.h"
#include "MapWindow/DataView/ObjectProxy.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    class ObjectEditor {
    public:
        explicit ObjectEditor(const EventContext& ctx = {}) : ctx_(ctx) {};
        ~ObjectEditor() = default;

        void renderGui(ObjectProxyPtr proxy);

    protected:
        const EventContext& ctx_;
    };
} // namespace Satisfactory3DMap::UI
