#ifndef SATISFACTORY3DMAP_SAVEHEADER_H
#define SATISFACTORY3DMAP_SAVEHEADER_H

#include <cstdint>
#include <istream>
#include <memory>

namespace Satisfactory3DMap {

    // See FGSaveSystem.h
    class SaveHeader {
    public:
        explicit SaveHeader(std::istream& stream);

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

        void print() const;

    protected:
        int32_t save_header_version_;
        int32_t save_version_;
        int32_t build_version_;
        std::string map_name_;
        std::string map_options_;
        std::string session_name_;
        int32_t play_duration_;
        int64_t save_date_time_;
        int8_t session_visibility_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEHEADER_H
