#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Satisfactory/FluidBox.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/InventoryItem.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/Mods/LBBalancerIndexing.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/ObjectReference.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/RailroadTrackPosition.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/SaveHeader.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_Satisfactory(py::module_& m) {
    py::class_<s::FFluidBox>(m, "FFluidBox")
        .def(py::init<>())
        .def_readwrite("Value", &s::FFluidBox::Value);

    py::class_<s::FInventoryItem>(m, "FInventoryItem")
        .def(py::init<>())
        .def_readwrite("unk1_", &s::FInventoryItem::unk1_)
        .def_readwrite("class_name_", &s::FInventoryItem::class_name_)
        .def_readwrite("ref_", &s::FInventoryItem::ref_);

    py::class_<s::FObjectReferenceDisc>(m, "FObjectReferenceDisc")
        .def(py::init<>())
        .def(py::init<>([](std::string levelName, std::string pathName) {
            return s::FObjectReferenceDisc(std::move(levelName), std::move(pathName));
        }))
        .def_readwrite("LevelName", &s::FObjectReferenceDisc::LevelName)
        .def_readwrite("PathName", &s::FObjectReferenceDisc::PathName);

    py::class_<s::FRailroadTrackPosition>(m, "FRailroadTrackPosition")
        .def(py::init<>())
        .def_readwrite("Track", &s::FRailroadTrackPosition::Track)
        .def_readwrite("Offset", &s::FRailroadTrackPosition::Offset)
        .def_readwrite("Forward", &s::FRailroadTrackPosition::Forward);

    py::class_<s::FSaveHeader>(m, "FSaveHeader")
        .def(py::init<>())
        .def_readwrite("SaveHeaderVersion", &s::FSaveHeader::SaveHeaderVersion)
        .def_readwrite("SaveVersion", &s::FSaveHeader::SaveVersion)
        .def_readwrite("BuildVersion", &s::FSaveHeader::BuildVersion)
        .def_readwrite("MapName", &s::FSaveHeader::MapName)
        .def_readwrite("MapOptions", &s::FSaveHeader::MapOptions)
        .def_readwrite("SessionName", &s::FSaveHeader::SessionName)
        .def_readwrite("PlayDurationSeconds", &s::FSaveHeader::PlayDurationSeconds)
        .def_readwrite("SaveDateTime", &s::FSaveHeader::SaveDateTime)
        .def_readwrite("SessionVisibility", &s::FSaveHeader::SessionVisibility)
        .def_readwrite("EditorObjectVersion", &s::FSaveHeader::EditorObjectVersion)
        .def_readwrite("ModMetadata", &s::FSaveHeader::ModMetadata)
        .def_readwrite("IsModdedSave", &s::FSaveHeader::IsModdedSave)
        .def_readwrite("SaveIdentifier", &s::FSaveHeader::SaveIdentifier)
        .def_readwrite("IsPartitionedWorld", &s::FSaveHeader::IsPartitionedWorld)
        .def_readwrite("SaveDataHash", &s::FSaveHeader::SaveDataHash)
        .def_readwrite("IsCreativeModeEnabled", &s::FSaveHeader::IsCreativeModeEnabled)
        .def("toString", &s::FSaveHeader::toString);

    py::class_<s::FLBBalancerIndexing>(m, "FLBBalancerIndexing")
        .def(py::init<>())
        .def_readwrite("mNormalIndex", &s::FLBBalancerIndexing::mNormalIndex)
        .def_readwrite("mOverflowIndex", &s::FLBBalancerIndexing::mOverflowIndex)
        .def_readwrite("mFilterIndex", &s::FLBBalancerIndexing::mFilterIndex);
}
