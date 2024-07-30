#pragma once

#include <cstdint>
#include <vector>

#include "GameTypes/Save/SaveObjectBase.h"
#include "GameTypes/UE/Satisfactory/ObjectReference.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "Utils/TimeMeasure.h"

namespace SatisfactorySave {
    static void parseTOCBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects,
        std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool& has_destroyedActorsTOC) {
        const auto TOC_size = ar.read<int64_t>();
        const auto TOC_pos = ar.tell();

        // Parse objects
        TIME_MEASURE_START("ObjHead");
        const auto num_objects = ar.read<int32_t>();
        saveObjects.reserve(num_objects);
        for (int32_t i = 0; i < num_objects; i++) {
            saveObjects.emplace_back(SaveObjectBase::create(ar));
        }
        TIME_MEASURE_END("ObjHead");

        // TODO ???
        has_destroyedActorsTOC = ar.tell() - TOC_pos < TOC_size;
        if (has_destroyedActorsTOC) {
            ar << destroyedActorsTOC;
        }

        if (ar.tell() - TOC_pos != TOC_size) {
            throw std::runtime_error("Invalid size of TOCBlob!");
        }
    }

    static void parseDataBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects) {
        const auto data_size = ar.read<int64_t>();
        const auto data_pos = ar.tell();

        // Parse object properties
        TIME_MEASURE_START("ObjProp");
        const auto num_object_data = ar.read<int32_t>();
        if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
            throw std::runtime_error("Bad number of object data!");
        }
        // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
        for (int32_t i = 0; i < num_object_data; i++) {
            ar << saveObjects[i]->SaveVersion;
            ar << saveObjects[i]->ShouldMigrateObjectRefsToPersistent;

            // Check stream pos to validate parser.
            const auto length = ar.read<int32_t>();
            auto pos_before = ar.tell();
            saveObjects[i]->serializeProperties(ar, length);
            auto pos_after = ar.tell();
            if (pos_after - pos_before != length) {
                throw std::runtime_error("Error parsing object data!");
            }
        }
        TIME_MEASURE_END("ObjProp");

        if (ar.tell() - data_pos != data_size) {
            throw std::runtime_error("Invalid size of DataBlob!");
        }
    }

    static void saveTOCBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects,
        std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool has_destroyedActorsTOC) {
        auto pos_size = ar.tell();
        ar.write<int64_t>(0);

        auto pos_before = ar.tell();

        // Write objects
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            ar.write(obj->isActor());
            ar << *obj;
        }

        if (has_destroyedActorsTOC) {
            ar << destroyedActorsTOC;
        }

        auto pos_after = ar.tell();

        ar.seek(pos_size);
        ar.write(static_cast<int64_t>(pos_after - pos_before));
        ar.seek(pos_after);
    }

    static void saveDataBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects) {
        auto blob_pos_size = ar.tell();
        ar.write<int64_t>(0);

        auto blob_pos_before = ar.tell();

        // Write object properties
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            ar << obj->SaveVersion;
            ar << obj->ShouldMigrateObjectRefsToPersistent;

            auto pos_size = ar.tell();
            ar.write<int32_t>(0);

            auto pos_before = ar.tell();
            obj->serializeProperties(ar, 0);
            auto pos_after = ar.tell();

            ar.seek(pos_size);
            ar.write(static_cast<int32_t>(pos_after - pos_before));
            ar.seek(pos_after);
        }

        auto blob_pos_after = ar.tell();

        ar.seek(blob_pos_size);
        ar.write(static_cast<int64_t>(blob_pos_after - blob_pos_before));
        ar.seek(blob_pos_after);
    }
} // namespace SatisfactorySave
