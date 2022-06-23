#include "PathSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::PathSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::PathSetting::serializeFromJson(const nlohmann::json& j) {
    std::filesystem::path path;
    j.get_to(path);
    if (validate(path)) {
        value_ = path;
    }
}

void Satisfactory3DMap::PathSetting::serializeToJson(nlohmann::json& j) {
    j = value_.string();
}

bool Satisfactory3DMap::PathSetting::validate(const std::filesystem::path& path) {
    // Allow empty path
    if (path.empty()) {
        return true;
    }
    if (type_ == PathType::Directory) {
        return std::filesystem::is_directory(path);
    } else {
        return std::filesystem::is_regular_file(path);
    }
}
