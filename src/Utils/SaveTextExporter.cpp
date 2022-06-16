#include "SaveTextExporter.h"

#include <fstream>

#include "GameTypes/Properties/PropertyVisitor.h"

namespace {
    class PropertyValueWriter : public Satisfactory3DMap::PropertyVisitor {
    private:
        std::ofstream& file_;

    public:
        explicit PropertyValueWriter(std::ofstream& file) : file_(file) {}

        void visit(Satisfactory3DMap::ArrayProperty& p) override {
            file_ << "  " << p.arrayType();
            // TODO values
        }

        void visit(Satisfactory3DMap::BoolProperty& p) override {
            file_ << "  " << static_cast<int>(p.value());
        }

        void visit(Satisfactory3DMap::ByteProperty& p) override {
            file_ << "  ByteType: " << p.byteType() << "  ";
            if (p.byteType() == "None") {
                file_ << static_cast<int>(p.valueByte());
            } else {
                file_ << p.valueName();
            }
        }

        void visit(Satisfactory3DMap::EnumProperty& p) override {
            file_ << "  EnumType: " << p.enumType() << "  " << p.value();
        }

        void visit(Satisfactory3DMap::FloatProperty& p) override {
            file_ << "  " << p.value();
        }

        void visit(Satisfactory3DMap::Int64Property& p) override {
            file_ << "  " << p.value();
        }

        void visit(Satisfactory3DMap::Int8Property& p) override {
            file_ << "  " << static_cast<int>(p.value());
        }

        void visit(Satisfactory3DMap::IntProperty& p) override {
            file_ << "  " << p.value();
        }

        void visit(Satisfactory3DMap::MapProperty& p) override {
            file_ << "  " << p.keyType() << "  " << p.valueType();
            // TODO values
        }

        void visit(Satisfactory3DMap::NameProperty& p) override {
            file_ << "  " << p.value();
        }

        void visit(Satisfactory3DMap::ObjectProperty& p) override {
            file_ << "  Lvl: " << p.value().levelName() << "  Path: " << p.value().pathName();
        }

        void visit(Satisfactory3DMap::SetProperty& p) override {
            file_ << "  " << p.setType();
            // TODO values
        }

        void visit(Satisfactory3DMap::StrProperty& p) override {
            file_ << "  " << p.value();
        }

        void visit(Satisfactory3DMap::StructProperty& p) override {
            file_ << "  " << p.structName() << "  " << p.guid().toString();
            // TODO values
        }

        void visit([[maybe_unused]] Satisfactory3DMap::TextProperty& p) override {
            file_ << "  " << p.textString();
        }
    };

    void writeObjects(std::ofstream& file,
        const std::vector<std::shared_ptr<Satisfactory3DMap::SaveObjectBase>>& saveObjects,
        std::vector<Satisfactory3DMap::ObjectReference> destroyedActors) {
        // Objects
        file << "=== objects ===" << std::endl;
        for (const auto& obj : saveObjects) {
            file << obj->className() << "::" << obj->reference().pathName() << " [" << (obj->type() == 1 ? "A" : "O")
                 << "]" << std::endl;

            for (const auto& p : obj->properties()) {
                file << "    " << p->name() << "  " << p->type();
                PropertyValueWriter w(file);
                p->accept(w);
                file << std::endl;
            }

            if (!obj->extraProperties().empty()) {
                file << "    EXTRA SIZE:" << obj->extraProperties().size() << std::endl;
            }
        }
        file << std::endl;

        // Destroyed actors
        file << "=== destroyed actors ===" << std::endl;
        for (const auto& obj : destroyedActors) {
            file << obj.levelName() << "  " << obj.pathName() << std::endl;
        }
        file << std::endl;
    }
} // namespace

void Satisfactory3DMap::saveToTextFile(const SaveGame& savegame, const std::string& filename) {
    std::ofstream file(filename);

    // Header
    file << "=== header ===" << std::endl;
    file << savegame.header().toString() << std::endl;

    file << "=== levels [" << savegame.levelData().size() << "] ===" << std::endl;
    for (auto& lvl : savegame.levelData()) {
        writeObjects(file, lvl.save_objects, lvl.destroyed_actors_TOC);
    }

    file << "=== main level ===" << std::endl;
    writeObjects(file, savegame.saveObjects(), savegame.destroyedActors());

    // Unresolved world data
    file << "=== unresolved world data ===" << std::endl;
    for (const auto& obj : savegame.unresolvedWorldSaveData()) {
        file << obj.levelName() << "  " << obj.pathName() << std::endl;
    }

    file.close();
}
