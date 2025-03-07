#pragma once

#include <iomanip>
#include <sstream>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FGuid {
    public:
        FGuid() : a_(0), b_(0), c_(0), d_(0) {}

        void serialize(Archive& ar) {
            ar << a_;
            ar << b_;
            ar << c_;
            ar << d_;
        }

        [[nodiscard]] bool isZero() const {
            return a_ == 0 && b_ == 0 && c_ == 0 && d_ == 0;
        }

        [[nodiscard]] std::string toString() const {
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
} // namespace SatisfactorySave
