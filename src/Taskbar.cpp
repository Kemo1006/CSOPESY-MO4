#include "Taskbar.h"
#include <cstdio>

Taskbar::Taskbar() {
}

Taskbar::~Taskbar() {
}

// Icons are drawn with ImGui draw commands (no image assets needed). `center` is the icon's middle.
void Taskbar::DrawIcon(ImDrawList* dl, IconKind icon, ImVec2 c, ImU32 col) {
    switch (icon) {
        case ICON_FOLDER:
            dl->AddRectFilled(ImVec2(c.x - 12, c.y - 10), ImVec2(c.x - 1, c.y - 5), col, 2.0f);   // tab
            dl->AddRectFilled(ImVec2(c.x - 12, c.y - 7), ImVec2(c.x + 12, c.y + 9), col, 3.0f);   // body
            break;
        case ICON_TERMINAL:
            dl->AddRect(ImVec2(c.x - 12, c.y - 9), ImVec2(c.x + 12, c.y + 9), col, 3.0f, 0, 2.0f);
            dl->AddLine(ImVec2(c.x - 7, c.y - 3), ImVec2(c.x - 3, c.y), col, 2.0f);                // ">"
            dl->AddLine(ImVec2(c.x - 3, c.y), ImVec2(c.x - 7, c.y + 3), col, 2.0f);
            dl->AddLine(ImVec2(c.x, c.y + 4), ImVec2(c.x + 6, c.y + 4), col, 2.0f);                // "_"
            break;
        case ICON_TASKMGR: {
            dl->AddLine(ImVec2(c.x - 12, c.y - 10), ImVec2(c.x - 12, c.y + 10), col, 2.0f);        // axes
            dl->AddLine(ImVec2(c.x - 12, c.y + 10), ImVec2(c.x + 12, c.y + 10), col, 2.0f);
            ImVec2 pts[5] = { ImVec2(c.x - 10, c.y + 4), ImVec2(c.x - 5, c.y - 3), ImVec2(c.x, c.y + 2),
                              ImVec2(c.x + 5, c.y - 8), ImVec2(c.x + 11, c.y - 2) };
            dl->AddPolyline(pts, 5, col, 0, 2.0f);                                                 // graph line
            break;
        }
        case ICON_INFO:
            dl->AddCircle(c, 11.0f, col, 24, 2.0f);
            dl->AddCircleFilled(ImVec2(c.x, c.y - 5), 1.8f, col);                                  // "i"
            dl->AddLine(ImVec2(c.x, c.y - 1), ImVec2(c.x, c.y + 6), col, 2.5f);
            break;
    }
}

void Taskbar::RenderIconButton(const char* id, const char* tooltip, IconKind icon, bool* windowState, ImVec4 accent) {
    const ImVec2 size(52.0f, 40.0f);
    bool isOpen = windowState && *windowState;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    if (ImGui::InvisibleButton(id, size) && windowState) {
        *windowState = !*windowState;
    }
    bool hovered = ImGui::IsItemHovered();
    bool held = ImGui::IsItemActive();

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 max(pos.x + size.x, pos.y + size.y);

    ImU32 bg = IM_COL32(255, 255, 255, 0);
    if (held)         bg = IM_COL32(255, 255, 255, 40);
    else if (hovered) bg = IM_COL32(255, 255, 255, 25);
    else if (isOpen)  bg = IM_COL32(255, 255, 255, 14);
    dl->AddRectFilled(pos, max, bg, 6.0f);

    ImVec4 iconColor = isOpen ? accent : ImVec4(0.80f, 0.85f, 0.92f, 1.0f);
    DrawIcon(dl, icon, ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f - 2.0f), ImGui::ColorConvertFloat4ToU32(iconColor));

    // Running-application indicator: a pill under the icon of every open app.
    if (isOpen) {
        dl->AddRectFilled(ImVec2(pos.x + 14, max.y - 5), ImVec2(max.x - 14, max.y - 2),
                          ImGui::ColorConvertFloat4ToU32(accent), 2.0f);
    }

    if (hovered) {
        ImGui::SetTooltip("%s%s", tooltip, isOpen ? "  (running)" : "");
    }
}

void Taskbar::UpdateAndRender(bool* showTaskManager, bool* showApp1, bool* showApp2, bool* showWidget) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float taskbarHeight = 52.0f;

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - taskbarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, taskbarHeight));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 6.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.10f, 0.16f, 0.95f));

    if (ImGui::Begin("##TaskbarPanel", NULL, flags)) {
        // Two unique app screens, then the Task Manager, then the System Info widget.
        RenderIconButton("##app1", "File Explorer", ICON_FOLDER, showApp1, ImVec4(0.98f, 0.78f, 0.25f, 1.0f));
        ImGui::SameLine();
        RenderIconButton("##app2", "Terminal", ICON_TERMINAL, showApp2, ImVec4(0.30f, 0.90f, 0.45f, 1.0f));
        ImGui::SameLine();
        RenderIconButton("##taskmgr", "Task Manager", ICON_TASKMGR, showTaskManager, ImVec4(0.98f, 0.55f, 0.20f, 1.0f));
        ImGui::SameLine();
        RenderIconButton("##sysinfo", "System Info", ICON_INFO, showWidget, ImVec4(0.65f, 0.45f, 0.95f, 1.0f));

        // Right side label
        const char* label = "CSOPESY OS Shell Dock";
        ImGui::SameLine();
        ImGui::SetCursorPosX(viewport->Size.x - ImGui::CalcTextSize(label).x - 16.0f);
        ImGui::SetCursorPosY(18.0f);
        ImGui::TextDisabled("%s", label);
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
}
