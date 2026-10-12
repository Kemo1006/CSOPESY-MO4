#ifndef TASKBAR_H
#define TASKBAR_H

#include "imgui.h"

class Taskbar {
public:
    Taskbar();
    ~Taskbar();

    void UpdateAndRender(bool* showTaskManager, bool* showApp1, bool* showApp2, bool* showWidget);

private:
    enum IconKind { ICON_FOLDER, ICON_TERMINAL, ICON_TASKMGR, ICON_INFO };

    void RenderIconButton(const char* id, const char* tooltip, IconKind icon, bool* windowState, ImVec4 accent);
    void DrawIcon(ImDrawList* dl, IconKind icon, ImVec2 center, ImU32 color);
};

#endif // TASKBAR_H
