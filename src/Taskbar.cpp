#include "Taskbar.h"
#include <cstdio>

Taskbar::Taskbar() {
}

Taskbar::~Taskbar() {
}

void Taskbar::RenderTaskbarButton(const char* label, const char* iconStr, bool* windowState, ImVec4 activeColor) {
    bool isWindowOpen = windowState ? *windowState : false;

    if (isWindowOpen) {
        ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(activeColor.x * 1.2f, activeColor.y * 1.2f, activeColor.z * 1.2f, 1.0f));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.16f, 0.24f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.26f, 0.38f, 1.0f));
    }

    char btnText[128];
    snprintf(btnText, sizeof(btnText), "%s %s", iconStr, label);

    if (ImGui::Button(btnText, ImVec2(140.0f, 36.0f))) {
        if (windowState) {
            *windowState = !(*windowState);
        }
    }

    ImGui::PopStyleColor(2);
}

void Taskbar::UpdateAndRender(bool* showTaskManager, bool* showApp1, bool* showApp2, bool* showWidget) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float taskbarHeight = 48.0f;

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - taskbarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, taskbarHeight));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoSavedSettings | 
                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 6.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.10f, 0.16f, 0.95f));

    if (ImGui::Begin("##TaskbarPanel", NULL, flags)) {
        // App 1 Button: File Explorer
        RenderTaskbarButton("File Explorer", "[DIR]", showApp1, ImVec4(0.18f, 0.48f, 0.78f, 1.0f));
        ImGui::SameLine();

        // App 2 Button: Terminal
        RenderTaskbarButton("Terminal", "[CMD]", showApp2, ImVec4(0.15f, 0.65f, 0.35f, 1.0f));
        ImGui::SameLine();

        // App 3 Button: Task Manager (Requirement check!)
        RenderTaskbarButton("Task Manager", "[TASK]", showTaskManager, ImVec4(0.82f, 0.45f, 0.12f, 1.0f));
        ImGui::SameLine();

        // App 4 Button: System Info Widget Toggle
        RenderTaskbarButton("System Info", "[SYS]", showWidget, ImVec4(0.55f, 0.25f, 0.75f, 1.0f));

        // Right side indicator text
        ImGui::SameLine();
        float rightPos = viewport->Size.x - 220.0f;
        ImGui::SetCursorPosX(rightPos);
        ImGui::TextDisabled("CSOPESY OS Shell Dock");
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}
