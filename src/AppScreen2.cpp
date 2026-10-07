#include "AppScreen2.h"
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <ctime>

AppScreen2::AppScreen2() 
    : m_scrollToBottom(false) {
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
    AddLog("=================================================");
    AddLog("  CSOPESY OS Interactive Terminal v2.0");
    AddLog("  Type 'help' for a list of available commands.");
    AddLog("=================================================");
}

AppScreen2::~AppScreen2() {
}

void AppScreen2::AddLog(const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    m_history.push_back(std::string(buf));
    m_scrollToBottom = true;
}

void AppScreen2::ExecuteCommand(const char* cmdStr) {
    AddLog("$ %s", cmdStr);

    std::string cmd(cmdStr);
    if (cmd == "help") {
        AddLog("Available Commands:");
        AddLog("  help     - Display this help menu");
        AddLog("  ver      - Display OS kernel version");
        AddLog("  ps       - List running processes");
        AddLog("  top      - Display CPU and Memory statistics");
        AddLog("  clock    - Output current system clock");
        AddLog("  clear    - Clear terminal buffer");
        AddLog("  exit     - Close terminal window");
    } else if (cmd == "ver") {
        AddLog("CSOPESY OS Kernel v2.0.26 [x86_64-mingw]");
        AddLog("Compositor Engine: Dear ImGui 1.90 + GLFW 3.4");
    } else if (cmd == "ps") {
        AddLog("PID   NAME                     CPU%%   MEM");
        AddLog("1004  csopesy_compositor.exe   4.2%%   128.5MB");
        AddLog("1088  system_idle_process.exe 82.1%%     4.0MB");
        AddLog("2140  dwm_compositor.exe       2.8%%    96.2MB");
        AddLog("3412  csopesy_terminal.exe     0.5%%    32.1MB");
    } else if (cmd == "top") {
        AddLog("CPU Load: 17.9%% | Memory Used: 4.8 GB / 16.0 GB");
        AddLog("Active Threads: 42 | Handles: 1,024");
    } else if (cmd == "clock") {
        time_t rawTime = time(NULL);
        struct tm* tInfo = localtime(&rawTime);
        char buf[64];
        if (tInfo) {
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tInfo);
            AddLog("Current System Time: %s", buf);
        }
    } else if (cmd == "clear") {
        m_history.clear();
    } else if (cmd == "exit") {
        AddLog("Closing terminal session...");
    } else if (cmd.rfind("echo ", 0) == 0) {
        AddLog("%s", cmd.length() >= 5 ? cmd.c_str() + 5 : "");
    } else if (!cmd.empty()) {
        AddLog("Unknown command '%s'. Type 'help' for command list.", cmdStr);
    }

    m_scrollToBottom = true;
}

void AppScreen2::UpdateAndRender(bool* pOpen) {
    if (!pOpen || !*pOpen) return;

    ImGui::SetNextWindowSize(ImVec2(550, 360), ImGuiCond_FirstUseEver);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.04f, 0.07f, 0.95f));

    if (ImGui::Begin("CSOPESY System Terminal", pOpen, ImGuiWindowFlags_NoCollapse)) {
        // Output log scroll region
        float footerHeight = ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.95f, 0.4f, 1.0f)); // Matrix Green terminal text
        for (size_t i = 0; i < m_history.size(); ++i) {
            ImGui::TextUnformatted(m_history[i].c_str());
        }
        ImGui::PopStyleColor();

        if (m_scrollToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
            m_scrollToBottom = false;
        }
        ImGui::EndChild();

        ImGui::Separator();

        // Prompt input field
        ImGui::Text("$");
        ImGui::SameLine();
        ImGui::PushItemWidth(-1.0f);
        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("##TerminalInput", m_inputBuffer, sizeof(m_inputBuffer), inputFlags)) {
            if (m_inputBuffer[0] != '\0') {
                if (std::string(m_inputBuffer) == "exit") {
                    *pOpen = false;
                } else {
                    ExecuteCommand(m_inputBuffer);
                }
                m_inputBuffer[0] = '\0';
            }
            ImGui::SetKeyboardFocusHere(-1); // Auto focus input box
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
    ImGui::PopStyleColor();
}
