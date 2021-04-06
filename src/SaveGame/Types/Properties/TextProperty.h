#ifndef SATISFACTORY3DMAP_TEXTPROPERTY_H
#define SATISFACTORY3DMAP_TEXTPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class TextProperty : public Property {
    public:
        TextProperty(std::string property_name, std::string property_type, std::istream& stream);

        void accept(PropertyVisitor& v) override;

        const std::vector<char>& buf() const {
            return buf_;
        }

    protected:
        std::vector<char> buf_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_TEXTPROPERTY_H
