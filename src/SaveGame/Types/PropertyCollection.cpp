#include "PropertyCollection.h"

SatisfactorySaveGame::PropertyCollection::PropertyCollection(int32_t length, std::istream& stream) {
    stream.ignore(length);
}
