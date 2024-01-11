#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MulticastSparseDelegateProperty : public Property {
        struct ScriptDelegate {
            FObjectReferenceDisc Object;
            FName FunctionName;

            void serialize(Archive& ar) {
                ar << Object;
                ar << FunctionName;
            }
        };

    public:
        static constexpr std::string_view TypeName = "MulticastSparseDelegateProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::vector<ScriptDelegate>& InvocationList() const {
            return InvocationList_;
        }

    protected:
        // TMulticastScriptDelegate
        // typedef TArray<TScriptDelegate<TWeakPtr>> FInvocationList;
        std::vector<ScriptDelegate> InvocationList_;
    };
} // namespace SatisfactorySave
