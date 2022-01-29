#ifndef SATISFACTORY3DMAP_STRUCT_H
#define SATISFACTORY3DMAP_STRUCT_H

#include <memory>
#include <utility>

#include "../Name.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    class StructVisitor;

    class Struct {
    public:
        static std::unique_ptr<Struct> create(const FName& struct_name, Archive& ar);

        explicit Struct(FName name) : name_(std::move(name)){};
        virtual ~Struct() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(StructVisitor& v) = 0;

        const FName& name() {
            return name_;
        }

    protected:
        FName name_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCT_H
