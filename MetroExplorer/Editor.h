#pragma once

#include <metro/MetroConfigDatabase.h>
#include <metro/VFXReader.h>
#include <metro/scripts/Script.h>

struct uobject;

enum class FileType {
    Unknown,
    Texture,
    Model,
    Level,
    Sound,
    LevelSpawn,
};

class Editor {
public:
    void SetRoot(const std::string& root);
    void SetCurrentFile(int curFile);
    void SetMainWnd(HWND wnd);
    void SetScript(const CharString& objName, const MyArray<Script>& scripts);
    void SetSelObj(uobject * obj);

    VFXReader            mFs;
    int                  mCurFile = -1;
    MetroConfigsDatabase mConfig;
    HWND                 mWnd = nullptr;
    CharString           mObjName;
    MyArray<Script>      mScripts;
    uobject*             mSelObj;
};

extern Editor gEditor;
