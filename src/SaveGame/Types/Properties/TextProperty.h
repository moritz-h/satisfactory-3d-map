#ifndef SATISFACTORY3DMAP_TEXTPROPERTY_H
#define SATISFACTORY3DMAP_TEXTPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class TextProperty : public Property {
    public:
        TextProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_TEXTPROPERTY_H
