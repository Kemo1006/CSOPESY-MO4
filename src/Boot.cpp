#include "Boot.h"
#include "Config.h"
#include "imgui.h"
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

static const float BIOS_HOLD = 0.8f;  // pause after the last BIOS line before loading starts

// "CSOPESY" in 5-row block letters, built by joining one glyph per letter.
static const char* const GLYPH_C[5] = { R"(  ____ )", R"( / ___|)", R"(| |    )", R"(| |___ )", R"( \____|)" };
static const char* const GLYPH_S[5] = { R"( ____  )", R"(/ ___| )", R"(\___ \ )", R"( ___) |)", R"(|____/ )" };
static const char* const GLYPH_O[5] = { R"(  ___  )", R"( / _ \ )", R"(| | | |)", R"(| |_| |)", R"( \___/ )" };
static const char* const GLYPH_P[5] = { R"( ____  )", R"(|  _ \ )", R"(| |_) |)", R"(|  __/ )", R"(|_|    )" };
static const char* const GLYPH_E[5] = { R"( _____ )", R"(| ____|)", R"(|  _|  )", R"(| |___ )", R"(|_____|)" };
static const char* const GLYPH_Y[5] = { R"(__   __)", R"(\ \ / /)", R"( \ V / )", R"(  | |  )", R"(  |_|  )" };

static const char* const BIOS_LINES[] = {
    "CSOPESY Megatrends",
    "Released: 12/01/94",
    "DLSU GAME Lab (C)1994 CSOPESY Megatrends Inc.",
    "",
    "",
    "Memory Test:",
    "Checking RAM : ",          // index 6: counts up, then " OK"
    "",
    "",
    "CPU Type: Pentium III",
    "BIOS Version: BCN SIT 1989-1994 Special CSOPESY",
    "Main Processor: Pentium III",
    "Numeric Processor: Built-in",
    "",
    "Primary Master:   420 MB WDCAC2420H",
    "Primary Slave:    None",
    "Secondary Master: CD-ROM LTN-305A",
    "Secondary Slave:  None",
    "",
    "Fun Fact: The CPU specs is a tribute to Bemani Python 2 Engine, the OS that powers up Dance Dance Revolution SuperNova series.",
    "It was essentially a retail Sony PlayStation 2 (SCPH-50000) inside a metal box with a specialized I/O board!",
};
static const int BIOS_LINE_COUNT = (int)(sizeof(BIOS_LINES) / sizeof(BIOS_LINES[0]));
static const int BIOS_RAM_LINE = 6;
static const int BIOS_RAM_KB = 64000;

BootSequence::BootSequence() : m_phase(PHASE_BIOS), m_phaseTime(0.0f) {
    if (!Config::Get().bootEnabled) m_phase = PHASE_DONE;
}

bool BootSequence::SkipRequested() const {
    if (!Config::Get().allowSkip) return false;
    return ImGui::IsKeyPressed(ImGuiKey_Space, false) ||
           ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
           ImGui::IsMouseClicked(0);
}

bool BootSequence::UpdateAndRender() {
    if (m_phase == PHASE_DONE) return true;

    const Config& cfg = Config::Get();
    m_phaseTime += ImGui::GetIO().DeltaTime;

    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 12.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav;

    bool advance = false;
    if (ImGui::Begin("##BootScreen", nullptr, flags)) {
        if (m_phase == PHASE_BIOS) {
            RenderBios(cfg.biosDuration);
            advance = m_phaseTime >= cfg.biosDuration + BIOS_HOLD;
        } else {
            RenderLoading(cfg.loadingDuration);
            advance = m_phaseTime >= cfg.loadingDuration;
        }
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);

    if (advance || SkipRequested()) {
        m_phase = (m_phase == PHASE_BIOS) ? PHASE_LOADING : PHASE_DONE;
        m_phaseTime = 0.0f;
    }
    return m_phase == PHASE_DONE;
}

void BootSequence::RenderBios(float duration) {
    // Lines are revealed one after another across the BIOS duration.
    float progress = std::fmin(m_phaseTime / duration, 1.0f) * BIOS_LINE_COUNT;
    int current = (int)progress;
    float frac = progress - (float)current;
    bool finished = m_phaseTime >= duration;

    const ImVec4 white(0.85f, 0.85f, 0.85f, 1.0f);
    for (int i = 0; i < BIOS_LINE_COUNT; ++i) {
        if (!finished && i > current) break;

        if (i == BIOS_RAM_LINE) {
            bool counting = !finished && i == current;
            if (counting) ImGui::TextColored(white, "%s%dK", BIOS_LINES[i], (int)(BIOS_RAM_KB * frac));
            else          ImGui::TextColored(white, "%s%dK OK", BIOS_LINES[i], BIOS_RAM_KB);
        } else if (BIOS_LINES[i][0] == '\0') {
            ImGui::Spacing();
        } else {
            ImGui::TextColored(white, "%s", BIOS_LINES[i]);
        }
    }

    // Blinking cursor under the text, like a real POST screen.
    if (std::fmod(m_phaseTime, 1.0f) < 0.5f) ImGui::TextColored(white, "_");
}

void BootSequence::RenderLoading(float duration) {
    ImVec2 area = ImGui::GetContentRegionAvail();
    auto centered = [&](const char* text, ImVec4 color) {
        float w = ImGui::CalcTextSize(text).x;
        ImGui::SetCursorPosX(std::fmax(0.0f, (ImGui::GetWindowWidth() - w) * 0.5f));
        ImGui::TextColored(color, "%s", text);
    };

    ImGui::SetCursorPosY(area.y * 0.22f);

    // Logo: join the 7 glyphs row by row.
    const char* const* glyphs[7] = { GLYPH_C, GLYPH_S, GLYPH_O, GLYPH_P, GLYPH_E, GLYPH_S, GLYPH_Y };
    const ImVec4 cyan(0.25f, 0.75f, 0.95f, 1.0f);
    for (int row = 0; row < 5; ++row) {
        std::string line;
        for (int g = 0; g < 7; ++g) line += glyphs[g][row];
        centered(line.c_str(), cyan);
    }

    const ImVec4 white(0.85f, 0.85f, 0.85f, 1.0f);
    ImGui::Spacing(); ImGui::Spacing();
    centered("CSOPESY Operating System Emulator v1.0", white);
    centered("Created By: Group 19", white);
    ImGui::Spacing();
    centered("MO4 Submission:", white);
    centered("Colcol, Tayzon, Ong", white);
    centered("AY 26-27 | Term 1", white);
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    int dots = 1 + ((int)(m_phaseTime * 2.5f) % 3);
    char loading[32];
    snprintf(loading, sizeof(loading), "Loading%.*s", dots, "...");
    centered(loading, ImVec4(0.3f, 0.95f, 0.4f, 1.0f));

    // Thin progress bar so the load time is visible.
    float pct = std::fmin(m_phaseTime / duration, 1.0f);
    float barW = 260.0f;
    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - barW) * 0.5f);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3f, 0.95f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::ProgressBar(pct, ImVec2(barW, 6.0f), "");
    ImGui::PopStyleColor(2);
}
