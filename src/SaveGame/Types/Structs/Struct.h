#ifndef SATISFACTORY3DMAP_STRUCT_H
#define SATISFACTORY3DMAP_STRUCT_H

#include <memory>
#include <string>
#include <utility>

namespace Satisfactory3DMap {

    class StructVisitor;

    class Struct {
    public:
        static std::unique_ptr<Struct> parse(const std::string& struct_name, std::istream& stream);

        Struct(std::string name) : name_(std::move(name)){};
        virtual ~Struct() = default;

        virtual void accept(StructVisitor& v) = 0;

        const std::string& name() {
            return name_;
        }

    protected:
        std::string name_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCT_H
