#include "stdafx.h"
#include "../Editor.h"
#include "ui.h"
#include <metro/entities/Entity.h>

namespace {
struct EntityPropsTool : public Tool {
    static Tool* create() {
        return new EntityPropsTool();
    }

    EntityPropsTool()
        : Tool(Tool::EntityProps) {
    }

    bool draw() override {
        if (mObj != gEditor.mSelObj)
            SetObj(gEditor.mSelObj);

        bool isOpen = true;
        if (ImGui::Begin(mTitle.c_str(), &isOpen)) {
            ImGui::Text("clsid: %lu", mObj->initData.clsid);
        }
        ImGui::End();
        return isOpen;
    }

    void SetObj(uobject* obj)
    {
        mObj = obj;
        mTitle = mObj->name + "###EntityProps";
    }

    CharString mTitle;
    uobject*   mObj = nullptr;
};

ToolRegistrator reg(Tool::EntityProps, &EntityPropsTool::create);

}
