#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "GameTypes/Save/SaveObject.h"
#include "GameTypes/UE/Satisfactory/ObjectReference.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "Utils/TimeMeasure.h"

namespace SatisfactorySave {
    template<typename toc_size_t = int64_t>
    inline void parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects,
        std::optional<std::vector<FObjectReferenceDisc>>& destroyedActorsTOC) {
        const auto TOC_size = ar.read<toc_size_t>();
        const auto TOC_pos = ar.tell();

        // Parse objects
        TIME_MEASURE_START("ObjHead");
        const auto num_objects = ar.read<int32_t>();
        saveObjects.clear();
        saveObjects.resize(num_objects);
        for (int32_t i = 0; i < num_objects; i++) {
            saveObjects[i] = std::make_shared<SaveObject>();
            saveObjects[i]->serializeTOC(ar);
        }
        TIME_MEASURE_END("ObjHead");

        // TODO ???
        if (ar.tell() - TOC_pos < TOC_size) {
            destroyedActorsTOC = std::vector<FObjectReferenceDisc>();
            ar << destroyedActorsTOC.value();
        }

        if (ar.tell() - TOC_pos != TOC_size) {
            throw std::runtime_error("Invalid size of TOCBlob!");
        }
    }

    template<typename data_size_t = int64_t, bool object_headers = true>
    inline void parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects) {
        const auto data_size = ar.read<data_size_t>();
        const auto data_pos = ar.tell();

        // Parse object properties
        TIME_MEASURE_START("ObjProp");
        const auto num_object_data = ar.read<int32_t>();
        if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
            throw std::runtime_error("Bad number of object data!");
        }
        // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
        for (int32_t i = 0; i < num_object_data; i++) {
            saveObjects[i]->serializeData(ar, object_headers);
        }
        TIME_MEASURE_END("ObjProp");

        if (ar.tell() - data_pos != data_size) {
            throw std::runtime_error("Invalid size of DataBlob!");
        }
    }

    template<typename toc_size_t = int64_t>
    inline void saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects,
        std::optional<std::vector<FObjectReferenceDisc>>& destroyedActorsTOC) {
        auto pos_size = ar.tell();
        ar.write<toc_size_t>(0);

        auto pos_before = ar.tell();

        // Write objects
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            obj->serializeTOC(ar);
        }

        if (destroyedActorsTOC.has_value()) {
            ar << destroyedActorsTOC.value();
        }

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
