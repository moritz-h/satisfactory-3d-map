#pragma once

#include <cstdint>
#include <string>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FGSaveSystem.h
    class SaveHeader {
    public:
        SaveHeader() = default;
        ~SaveHeader() = default;

        void serialize(Archive& ar);

        [[nodiscard]] int32_t saveHeaderVersion() const {
            return save_header_version_;
        }

        [[nodiscard]] int32_t saveVersion() const {
            return save_version_;
        }

        [[nodiscard]] int32_t buildVersion() const {
            return build_version_;
        }

        [[nodiscard]] const std::string& mapName() const {
            return map_name_;
        }

        [[nodiscard]] const std::string& mapOptions() const {
            return map_options_;
        }

        [[nodiscard]] const std::string& sessionName() const {
            return session_name_;
        }

        [[nodiscard]] int32_t playSuration() const {
            return play_duration_;
        }

        [[nodiscard]] int64_t saveDateTime() const {
            return save_date_time_;
        }

        [[nodiscard]] int8_t sessionVisibility() const {
            return session_visibility_;
        }

        [[nodiscard]] int32_t editorObjectVersion() const {
            return editor_object_version_;
        }

        [[nodiscard]] const std::string& modMetadata() const {
            return mod_metadata_;
        }

        [[nodiscard]] int32_t isModdedSave() const {
            return is_modded_save_;
        }

        [[nodiscard]] std::string toString() const;

    protected:
        int32_t save_header_version_ = 0;
        int32_t save_version_ = 0;
        int32_t build_version_ = 0;
        std::string map_name_;
        std::string map_options_;
        std::string session_name_;
        int32_t play_duration_ = 0;
        int64_t save_date_time_ = 0;
        int8_t session_visibility_ = 0;
        int32_t editor_object_version_ = 0;
        std::string mod_metadata_;
        int32_t is_modded_save_ = 0;
    };
} // namespace Satisfactory3DMap
