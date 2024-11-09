#pragma once

#include "../Satisfactory/ObjectReference.h"
#include "Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ScriptDelegate {
    public:
        FObjectReferenceDisc Object;
        FName FunctionName;

        void serialize(Archive& ar) {
            ar << Object;
            ar << FunctionName;
        }
    };

    class SATISFACTORYSAVE_API FMulticastScriptDelegate {
    public:
        std::vector<ScriptDelegate> InvocationList;

        void serialize(Archive& ar) {
            ar << InvocationList;
        }
    };

} // namespace SatisfactorySave
