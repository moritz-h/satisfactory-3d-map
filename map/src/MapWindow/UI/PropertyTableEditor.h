#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"

namespace Satisfactory3DMap {

    class PropertyTableEditor {
    public:
        PropertyTableEditor() = default;
        ~PropertyTableEditor() = default;

        void renderGui(const SatisfactorySave::PropertyList& properties,
            const std::function<void(const std::string&)>& callback);

    protected:
    };
} // namespace Satisfactory3DMap
