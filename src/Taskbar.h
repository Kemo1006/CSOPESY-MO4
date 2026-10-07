#ifndef TASKBAR_H
#define TASKBAR_H

#include "imgui.h"

class Taskbar {
public:
    Taskbar();
    ~Taskbar();

    void UpdateAndRender(bool* showTaskManager, bool* showApp1, bool* showApp2, bool* showWidget);

private:
    void RenderTaskbarButton(const char* label, const char* icon, bool* windowState, ImVec4 activeColor);
};

#endif // TASKBAR_H
