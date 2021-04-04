#ifndef SATISFACTORY3DMAP_GUID_H
#define SATISFACTORY3DMAP_GUID_H

#include <iomanip>
#include <sstream>

#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    // FGuid
    // https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FGuid/index.html
    class Guid {
    public:
        Guid() : a_(0), b_(0), c_(0), d_(0){};
        explicit Guid(std::istream& stream) {
            a_ = read<uint32_t>(stream);
            b_ = read<uint32_t>(stream);
            c_ = read<uint32_t>(stream);
            d_ = read<uint32_t>(stream);
        };

        bool isZero() const {
            return a_ == 0 && b_ == 0 && c_ == 0 && d_ == 0;
        }

        std::string toString() const {
            std::stringstream s;
            s << std::setfill('0') << std::setw(8) << std::hex << a_ << "-";
            s << std::setw(4) << (b_ >> 16) << "-";
            s << std::setw(4) << (b_ & 0xFFFF) << "-";
            s << std::setw(4) << (c_ >> 16) << "-";
            s << std::setw(4) << (c_ & 0xFFFF) << std::setw(8) << d_;
            return s.str();
        }

    private:
        uint32_t a_;
        uint32_t b_;
        uint32_t c_;
        uint32_t d_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_GUID_H
