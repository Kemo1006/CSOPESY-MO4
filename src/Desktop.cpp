#include "Desktop.h"
#include "Config.h"
#include <ctime>
#include <cstdio>
#include <cmath>

Desktop::Desktop() 
    : m_currentWallpaper(WALLPAPER_AURORA), m_animTime(0.0f) {
    int w = Config::Get().wallpaper;
    if (w >= 0 && w < WALLPAPER_COUNT) m_currentWallpaper = static_cast<WallpaperStyle>(w);
}

Desktop::~Desktop() {
}

void Desktop::CycleWallpaper() {
    m_currentWallpaper = static_cast<WallpaperStyle>((m_currentWallpaper + 1) % WALLPAPER_COUNT);
}

std::string Desktop::GetCurrentWallpaperName() const {
    switch (m_currentWallpaper) {
        case WALLPAPER_AURORA: return "Deep Aurora";
        case WALLPAPER_CYBERPUNK: return "Cyberpunk Grid";
        case WALLPAPER_MIDNIGHT: return "Midnight Blue";
        case WALLPAPER_SUNSET: return "Sunset Horizon";
        default: return "Default";
    }
}

void Desktop::RenderWallpaper(ImDrawList* drawList, float width, float height) {
    m_animTime += ImGui::GetIO().DeltaTime;

    if (m_currentWallpaper == WALLPAPER_AURORA) {
        // Deep Blue to Dark Violet Gradient
        ImU32 topColor = IM_COL32(15, 23, 42, 255);     // Dark Slate
        ImU32 bottomColor = IM_COL32(30, 10, 60, 255);  // Deep Purple
        drawList->AddRectFilledMultiColor(
            ImVec2(0, 0), ImVec2(width, height),
            topColor, topColor, bottomColor, bottomColor
        );

        // Animated glowing curves/aurora
        for (int i = 0; i < 5; ++i) {
            float offset = i * 120.0f;
            float waveY = height * 0.4f + sinf(m_animTime * 0.5f + i) * 60.0f;
            ImU32 lineCol = IM_COL32(56, 189, 248, 40 + i * 15); // Cyan glow
            drawList->AddCircleFilled(ImVec2(width * 0.3f + offset, waveY), 140.0f + i * 20.0f, lineCol, 64);
        }
    } 
    else if (m_currentWallpaper == WALLPAPER_CYBERPUNK) {
        // Dark Cyan to Magenta Grid
        ImU32 topColor = IM_COL32(10, 10, 25, 255);
        ImU32 bottomColor = IM_COL32(40, 5, 40, 255);
        drawList->AddRectFilledMultiColor(
            ImVec2(0, 0), ImVec2(width, height),
            topColor, topColor, bottomColor, bottomColor
        );

        // Grid lines
        float gridSize = 40.0f;
        ImU32 gridCol = IM_COL32(236, 72, 153, 35); // Pink glow
        for (float x = 0; x < width; x += gridSize) {
            drawList->AddLine(ImVec2(x, 0), ImVec2(x, height), gridCol, 1.0f);
        }
        for (float y = 0; y < height; y += gridSize) {
            drawList->AddLine(ImVec2(0, y), ImVec2(width, y), gridCol, 1.0f);
        }
    } 
    else if (m_currentWallpaper == WALLPAPER_MIDNIGHT) {
        // Deep Navy Gradient
        ImU32 topColor = IM_COL32(2, 6, 23, 255);
        ImU32 bottomColor = IM_COL32(15, 23, 42, 255);
        drawList->AddRectFilledMultiColor(
            ImVec2(0, 0), ImVec2(width, height),
            topColor, topColor, bottomColor, bottomColor
        );
    } 
    else if (m_currentWallpaper == WALLPAPER_SUNSET) {
        // Sunset Red/Orange to Dark Indigo
        ImU32 topColor = IM_COL32(24, 18, 43, 255);
        ImU32 bottomColor = IM_COL32(225, 29, 72, 255);
        drawList->AddRectFilledMultiColor(
            ImVec2(0, 0), ImVec2(width, height),
            topColor, topColor, bottomColor, bottomColor
        );
    }
}

void Desktop::RenderTopSystemBar(float width, bool* pwrShutdownRequested) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(width, 36.0f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoSavedSettings | 
                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.07f, 0.12f, 0.85f));

    if (ImGui::Begin("##TopSystemBar", NULL, flags)) {
        // OS Title
        ImGui::SetCursorPos(ImVec2(12.0f, 8.0f));
        ImGui::TextColored(ImVec4(0.38f, 0.69f, 0.98f, 1.0f), "[CSOPESY OS v1.0]");

        ImGui::SameLine();
        ImGui::TextDisabled("|  Desktop Compositor Engine");

        // Live clock + date
        time_t rawTime = time(NULL);
        struct tm* timeInfo = localtime(&rawTime);
        char clockStr[96];
        if (timeInfo) {
            strftime(clockStr, sizeof(clockStr),
                     Config::Get().clock24h ? "%A, %b %d, %Y | %H:%M:%S" : "%A, %b %d, %Y | %I:%M:%S %p",
                     timeInfo);
        } else {
            snprintf(clockStr, sizeof(clockStr), "--:--:--");
        }

        float clockWidth = ImGui::CalcTextSize(clockStr).x;
        ImGui::SetCursorPos(ImVec2(width - 100.0f - clockWidth - 12.0f, 10.0f));
        ImGui::TextColored(ImVec4(0.9f, 0.95f, 1.0f, 1.0f), "%s", clockStr);

        // PWR Shutdown Button
        ImGui::SetCursorPos(ImVec2(width - 90.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.18f, 0.22f, 0.85f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.25f, 0.30f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.10f, 0.15f, 1.0f));
        
        if (ImGui::Button(" [PWR] ", ImVec2(78.0f, 26.0f))) {
            ImGui::OpenPopup("Confirm Shutdown");
        }

        ImGui::PopStyleColor(3);

        // Modal Shutdown Dialog
        if (ImGui::BeginPopupModal("Confirm Shutdown", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to shut down CSOPESY OS?");
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button(" Shutdown ", ImVec2(120.0f, 30.0f))) {
                *pwrShutdownRequested = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button(" Cancel ", ImVec2(120.0f, 30.0f))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

void Desktop::RenderDesktopIcons(bool* showWidget) {
    if (!showWidget || !*showWidget) return;

    // Render decorative desktop widgets/shortcuts
    ImGui::SetNextWindowPos(ImVec2(20.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(240.0f, 180.0f), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("System Info Widget", showWidget, flags)) {
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.4f, 1.0f), "Status: Running");
        ImGui::Text("Kernel: CSOPESY-v1.0-x64");
        ImGui::Text("Compositor: GLFW + ImGui");
        ImGui::Separator();
        ImGui::Text("Current Theme:");
        ImGui::BulletText("%s", GetCurrentWallpaperName().c_str());

        if (ImGui::Button("Change Wallpaper", ImVec2(-1, 26.0f))) {
            CycleWallpaper();
        }
    }
    ImGui::End();
}

void Desktop::UpdateAndRender(bool* pwrShutdownRequested, bool* showWidget) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 workSize = viewport->Size;

    // 1. Render Full-Screen Desktop Background on ImGui's background draw list.
    // (A window with NoBringToFrontOnFocus gets sorted in front of the bars, hiding them.)
    RenderWallpaper(ImGui::GetBackgroundDrawList(), workSize.x, workSize.y);

    // 2. Render Top System Bar with Real-Time Clock & PWR Shutdown Button
    RenderTopSystemBar(workSize.x, pwrShutdownRequested);

    // 3. Render Desktop Widgets
    RenderDesktopIcons(showWidget);
}
