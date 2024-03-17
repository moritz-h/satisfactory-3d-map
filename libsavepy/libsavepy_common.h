#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>

#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/ObjectReference.h"
#include "SatisfactorySave/GameTypes/UE/UObject/Name.h"
#include "SatisfactorySave/GameTypes/UE/UObject/SoftObjectPath.h"

namespace s = SatisfactorySave;

PYBIND11_MAKE_OPAQUE(std::vector<int8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int64_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint64_t>);
PYBIND11_MAKE_OPAQUE(std::vector<float>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<std::string>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FName>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FObjectReferenceDisc>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FSoftObjectPath>);
PYBIND11_MAKE_OPAQUE(s::SaveGame::SaveObjectList);
PYBIND11_MAKE_OPAQUE(std::vector<std::unique_ptr<s::Struct>>);
