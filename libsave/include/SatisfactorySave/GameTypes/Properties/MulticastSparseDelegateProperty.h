#pragma once

#include "../ObjectReference.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class MulticastSparseDelegateProperty : public Property {
        struct ScriptDelegate {
            ObjectReference Object;
            FName FunctionName;

            void serialize(Archive& ar) {
                ar << Object;
                ar << FunctionName;
            }
        };

    public:
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
} // namespace Satisfactory3DMap
