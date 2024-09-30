#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "GameTypes/Save/SaveObjectBase.h"
#include "GameTypes/UE/Satisfactory/ObjectReference.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "Utils/TimeMeasure.h"

namespace SatisfactorySave {
    template<typename toc_size_t = int64_t>
    inline void parseTOCBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects,
        std::optional<std::vector<FObjectReferenceDisc>>& destroyedActorsTOC) {
        const auto TOC_size = ar.read<toc_size_t>();
        const auto TOC_pos = ar.tell();

        // Parse objects
        TIME_MEASURE_START("ObjHead");
        const auto num_objects = ar.read<int32_t>();
        saveObjects.clear();
        saveObjects.reserve(num_objects);
        for (int32_t i = 0; i < num_objects; i++) {
            saveObjects.emplace_back(SaveObjectBase::create(ar));
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
    inline void parseDataBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects) {
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
            std::unique_ptr<StackGuard<int32_t>> save_version_stack_pusher;
            if constexpr (object_headers) {
                ar << saveObjects[i]->SaveVersion;
                ar << saveObjects[i]->ShouldMigrateObjectRefsToPersistent;
                save_version_stack_pusher = ar.pushSaveVersion(saveObjects[i]->SaveVersion);
            }

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

    template<typename toc_size_t = int64_t>
    inline void saveTOCBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects,
        std::optional<std::vector<FObjectReferenceDisc>>& destroyedActorsTOC) {
        auto pos_size = ar.tell();
        ar.write<toc_size_t>(0);

        auto pos_before = ar.tell();

        // Write objects
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            ar.write(obj->isActor());
            ar << *obj;
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
    inline void saveDataBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects) {
        auto blob_pos_size = ar.tell();
        ar.write<data_size_t>(0);

        auto blob_pos_before = ar.tell();

        // Write object properties
        ar.write(static_cast<int32_t>(saveObjects.size()));
        for (const auto& obj : saveObjects) {
            std::unique_ptr<StackGuard<int32_t>> save_version_stack_pusher;
            if constexpr (object_headers) {
                ar << obj->SaveVersion;
                ar << obj->ShouldMigrateObjectRefsToPersistent;
                save_version_stack_pusher = ar.pushSaveVersion(obj->SaveVersion);
            }

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
        ar.write(static_cast<data_size_t>(blob_pos_after - blob_pos_before));
        ar.seek(blob_pos_after);
    }
} // namespace SatisfactorySave
