#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/Save/SaveObject.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "Utils/TimeMeasure.h"

namespace SatisfactorySave {
    template<typename T>
    concept OptionalReference =
        std::is_same_v<std::remove_reference_t<T>, std::optional<typename std::remove_reference_t<T>::value_type>>;

    // This function provides the following interface:
    //   parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects)
    //
    //   template<typename T>
    //   parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects, std::optional<T>& toc_extra_data)
    //
    // If `toc_extra_data` is provided, it is checked if the TOC buffer has data left after serializing saveObjects.
    // If yes, the data is serialized as T, if not the optional is set to std::nullopt.
    //
    // Implementation notes: To achieve the above function signature with either zero or one toc_extra_data parameter,
    // and without duplicating the code into different overload functions, we use a variadic template and additionally
    // static assert that at most one parameter is provided. A fold expression is used to execute the optional
    // serialization code. With C++26 this may could be simplified to constexpr if and pack indexing.
    template<typename toc_size_t = int64_t, OptionalReference... Args>
    inline void parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects, Args&... toc_extra_data) {
        static_assert(sizeof...(Args) <= 1, "Use only one toc_extra_data parameter!");

        const auto TOC_size = ar.read<toc_size_t>();
        const auto TOC_pos = ar.tell();

        // Parse objects
        TIME_MEASURE_START("ObjectTOC");
        const auto num_objects = ar.read<int32_t>();
        saveObjects.clear();
        saveObjects.resize(num_objects);
        for (int32_t i = 0; i < num_objects; i++) {
            saveObjects[i] = std::make_shared<SaveObject>();
            saveObjects[i]->serializeTOC(ar);
        }
        TIME_MEASURE_END("ObjectTOC");

        // This extra data seems always written by the current game code (UFGSaveSession::SaveLevelState), but never
        // seems to be read. However, there are save games that do not contain this data. The best guess is that
        // writing this data was added at some time in development, but save games may still contain the old state.
        // This guess is supported by analyzing the object SaveVersion in DataBlob64 in an old save game. TOCBlob64
        // having this data did correlate to object SaveVersion 46, while TOCBlob64 missing this data did correlate to
        // object SaveVersion 36.

        // clang-format off
        ([&]() {
            using T = typename std::remove_reference_t<Args>::value_type;
            if (ar.tell() - TOC_pos < TOC_size) {
                toc_extra_data = T();
                ar << toc_extra_data.value();
            } else {
                toc_extra_data = std::nullopt;
            }
        }(), ...);
        // clang-format on

        if (ar.tell() - TOC_pos != TOC_size) {
            throw std::runtime_error("Invalid size of TOCBlob!");
        }
    }

    template<typename data_size_t = int64_t, bool object_headers = true>
    inline void parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects) {
        const auto data_size = ar.read<data_size_t>();
        const auto data_pos = ar.tell();

        // Parse object properties
        TIME_MEASURE_START("ObjectData");
        const auto num_object_data = ar.read<int32_t>();
        if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
            throw std::runtime_error("Bad number of object data!");
        }
        // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
        for (int32_t i = 0; i < num_object_data; i++) {
            saveObjects[i]->serializeData(ar, object_headers);
        }
        TIME_MEASURE_END("ObjectData");

        if (ar.tell() - data_pos != data_size) {
            throw std::runtime_error("Invalid size of DataBlob!");
        }
    }

    template<typename toc_size_t = int64_t, OptionalReference... Args>
    inline void saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects, Args&... toc_extra_data) {
        static_assert(sizeof...(Args) <= 1, "Use only one toc_extra_data parameter!");

        auto pos_size = ar.tell();
        ar.write<toc_size_t>(0);

        auto pos_before = ar.tell();

        // Write objects
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            obj->serializeTOC(ar);
        }

        // clang-format off
        ([&]() {
            if (toc_extra_data.has_value()) {
                ar << toc_extra_data.value();
            }
        }(), ...);
        // clang-format on

        auto pos_after = ar.tell();

        ar.seek(pos_size);
        ar.write(static_cast<toc_size_t>(pos_after - pos_before));
        ar.seek(pos_after);
    }

    template<typename data_size_t = int64_t, bool object_headers = true>
    inline void saveDataBlob(OStreamArchive& ar, SaveObjectList& saveObjects) {
        auto blob_pos_size = ar.tell();
        ar.write<data_size_t>(0);

        auto blob_pos_before = ar.tell();

        // Write object properties
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            obj->serializeData(ar, object_headers);
        }

        auto blob_pos_after = ar.tell();

        ar.seek(blob_pos_size);
        ar.write(static_cast<data_size_t>(blob_pos_after - blob_pos_before));
        ar.seek(blob_pos_after);
    }
} // namespace SatisfactorySave
