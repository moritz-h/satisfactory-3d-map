#pragma once

#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FDateTime {
    public:
        int64_t Ticks = 0;

        void serialize(Archive& ar) {
            ar << Ticks;
        }

        [[nodiscard]] std::string toString() const {
            // save_date_time is integer ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds.
            // See: https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html
            // Satisfactory seems to use UTC.
            // Convert to unix timestamp:
            // Python: (datetime.datetime(1970, 1, 1) - datetime.datetime(1, 1, 1)).total_seconds()
            //   => 62135596800.0 seconds
            std::time_t save_date_time = (Ticks - 621355968000000000) / 10000000;

            // Convert values
            std::string save_date_str(20, '\0');
            std::strftime(save_date_str.data(), save_date_str.size(), "%F %T", std::localtime(&save_date_time));
            save_date_str.erase(std::find(save_date_str.begin(), save_date_str.end(), '\0'), save_date_str.end());
            return save_date_str;
        }
    };
} // namespace SatisfactorySave
