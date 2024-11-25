#include "Utils/SaveTextExporter.h"

#include <fstream>
#include <string>

#include "GameTypes/Properties/Base/PropertyAll.h"
#include "GameTypes/Properties/Base/PropertyVisitor.h"

namespace {
    class PropertyValueWriter : public SatisfactorySave::PropertyVisitor {
    private:
        std::ofstream& file_;

    public:
        explicit PropertyValueWriter(std::ofstream& file) : file_(file) {}

        void visit(SatisfactorySave::ArrayProperty& p) override {
            file_ << "  " << p.ArrayType();
            // TODO values
        }

        void visit(SatisfactorySave::BoolProperty& p) override {
            file_ << "  " << static_cast<int>(p.getValue());
        }

        void visit(SatisfactorySave::ByteProperty& p) override {
            file_ << "  ByteType: " << p.EnumName() << "  ";
            if (p.EnumName() == "None") {
                file_ << static_cast<int>(std::get<int8_t>(p.Value));
            } else {
                file_ << std::get<SatisfactorySave::FName>(p.Value);
            }
        }

        void visit(SatisfactorySave::DoubleProperty& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::EnumProperty& p) override {
            file_ << "  EnumType: " << p.EnumName() << "  " << p.Value;
        }

        void visit(SatisfactorySave::FloatProperty& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::Int64Property& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::Int8Property& p) override {
            file_ << "  " << static_cast<int>(p.Value);
        }

        void visit(SatisfactorySave::IntProperty& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::MapProperty& p) override {
            file_ << "  " << p.KeyType() << "  " << p.ValueType();
            // TODO values
        }

        void visit([[maybe_unused]] SatisfactorySave::MulticastSparseDelegateProperty& p) override {
            // only used in pak
        }

        void visit(SatisfactorySave::NameProperty& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::ObjectProperty& p) override {
            file_ << "  Lvl: " << p.Value.LevelName << "  Path: " << p.Value.PathName;
        }

        void visit(SatisfactorySave::SetProperty& p) override {
            file_ << "  " << p.SetType();
            // TODO values
        }

        void visit(SatisfactorySave::SoftObjectProperty& p) override {
            file_ << "  AssetPath.PackageName: " << p.Value.AssetPath.PackageName.toString()
                  << "  AssetPath.AssetName:   " << p.Value.AssetPath.AssetName.toString()
                  << "  SubPathString:         " << p.Value.SubPathString;
        }

        void visit(SatisfactorySave::StrProperty& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::StructProperty& p) override {
            file_ << "  " << p.StructName() << "  " << p.StructGuid().toString();
            // TODO values
        }

        void visit(SatisfactorySave::TextProperty& p) override {
            file_ << "  " << p.Value.string();
        }

        void visit(SatisfactorySave::UInt32Property& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::UInt64Property& p) override {
            file_ << "  " << p.Value;
        }

        void visit(SatisfactorySave::UnknownProperty& p) override {
            file_ << "  [UnknownProperty] " << p.Type();
        }
    };

    void writeObjects(std::ofstream& file,
        const std::vector<std::shared_ptr<SatisfactorySave::SaveObject>>& saveObjects,
        const std::optional<std::vector<SatisfactorySave::FObjectReferenceDisc>>& destroyedActors) {
        // Objects
        file << "=== objects ===" << std::endl;
        for (const auto& obj : saveObjects) {
            file << obj->baseHeader().ClassName << "::" << obj->baseHeader().Reference.PathName << " ["
                 << (obj->isActor() ? "A" : "O") << "]" << std::endl;

            for (const auto& p : obj->Object->Properties) {
                file << "    " << p->Name() << "  " << p->Type();
                PropertyValueWriter w(file);
                p->accept(w);
                file << std::endl;
            }

            if (!obj->BinaryClassData.empty()) {
                file << "    EXTRA SIZE:" << obj->BinaryClassData.size() << std::endl;
            }
        }
        file << std::endl;

        // Destroyed actors
        if (destroyedActors.has_value()) {
            file << "=== destroyed actors ===" << std::endl;
            for (const auto& obj : destroyedActors.value()) {
                file << obj.LevelName << "  " << obj.PathName << std::endl;
            }
            file << std::endl;
        }
    }
} // namespace

void SatisfactorySave::saveToTextFile(const SaveGame& savegame, const std::filesystem::path& filename) {
    std::ofstream file(filename);

    // Header
    file << "=== header ===" << std::endl;
    file << savegame.mSaveHeader.toString() << std::endl;

    file << "=== levels [" << savegame.mPerLevelDataMap.size() << "] ===" << std::endl;
    for (auto& lvl : savegame.mPerLevelDataMap.Values) {
        writeObjects(file, lvl.SaveObjects, lvl.TOC_DestroyedActors);
    }

    file << "=== main level ===" << std::endl;
    writeObjects(file, savegame.mPersistentAndRuntimeData.SaveObjects,
        std::nullopt); // TODO savegame.mPersistentAndRuntimeData.TOC_LevelToDestroyedActorsMap

    // Unresolved world data
    file << "=== unresolved world data ===" << std::endl;
    for (const auto& obj : savegame.mUnresolvedWorldSaveData.DestroyedActors) {
        file << obj.LevelName << "  " << obj.PathName << std::endl;
    }

    file.close();
}
