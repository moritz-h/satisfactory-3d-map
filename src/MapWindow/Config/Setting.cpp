#include "Setting.h"

#include "Configuration.h"

void Satisfactory3DMap::Setting::update() {
    if (const auto cfg = config_.lock()) {
        cfg->requestSave();
    }
}
