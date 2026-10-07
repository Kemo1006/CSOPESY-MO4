#include "TaskManager.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

TaskManager::TaskManager() 
    : m_selectedProcessIndex(-1), m_updateTimer(0.0f) {
    InitializeProcesses();

    // Initialize performance histories (50 data points)
    m_cpuHistory.resize(50, 15.0f);
    m_ramHistory.resize(50, 42.0f);
}

TaskManager::~TaskManager() {
}

void TaskManager::InitializeProcesses() {
    m_processes.clear();
    m_processes.push_back({ 1004, "csopesy_compositor.exe", 4.2f, 128.5f, "Running", "SYSTEM" });
    m_processes.push_back({ 1088, "system_idle_process.exe", 82.1f, 4.0f, "Running", "SYSTEM" });
    m_processes.push_back({ 2140, "dwm_compositor.exe", 2.8f, 96.2f, "Running", "SYSTEM" });
    m_processes.push_back({ 3412, "csopesy_terminal.exe", 0.5f, 32.1f, "Running", "Student" });
    m_processes.push_back({ 4096, "file_explorer.exe", 1.1f, 64.8f, "Running", "Student" });
    m_processes.push_back({ 5120, "imgui_renderer.exe", 3.4f, 112.0f, "Running", "Student" });
    m_processes.push_back({ 6234, "antigravity_agent.exe", 1.8f, 184.2f, "Running", "Student" });
    m_processes.push_back({ 7800, "audio_service.exe", 0.2f, 18.4f, "Running", "LOCAL SERVICE" });
}

void TaskManager::UpdateTelemetry(float deltaTime) {
    m_updateTimer += deltaTime;

    // Periodically fluctuate dummy CPU and Memory values for real-time feel
    if (m_updateTimer >= 0.5f) {
        m_updateTimer = 0.0f;
        float totalCpu = 0.0f;
        float totalRam = 0.0f;

        for (size_t i = 0; i < m_processes.size(); ++i) {
            if (m_processes[i].name != "system_idle_process.exe") {
                float cpuDelta = ((rand() % 100) - 50) / 20.0f;
                m_processes[i].cpuUsage = std::max(0.1f, std::min(45.0f, m_processes[i].cpuUsage + cpuDelta));

                float ramDelta = ((rand() % 100) - 50) / 10.0f;
                m_processes[i].memoryUsageMB = std::max(5.0f, m_processes[i].memoryUsageMB + ramDelta);

                totalCpu += m_processes[i].cpuUsage;
                totalRam += m_processes[i].memoryUsageMB;
            }
        }

        // Rotate performance history buffers
        m_cpuHistory.erase(m_cpuHistory.begin());
        m_cpuHistory.push_back(std::min(100.0f, totalCpu));

        m_ramHistory.erase(m_ramHistory.begin());
        m_ramHistory.push_back(std::min(100.0f, (totalRam / 16384.0f) * 100.0f + 25.0f));
    }
}

void TaskManager::RenderProcessesTab() {
    ImGui::Text("Active System Processes (%zu)", m_processes.size());
    ImGui::Separator();

    // Table with columns: PID, Name, CPU, Memory, Status, User
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | 
                                 ImGuiTableFlags_RowBg | 
                                 ImGuiTableFlags_Resizable | 
                                 ImGuiTableFlags_ScrollY | 
                                 ImGuiTableFlags_Sortable;

    if (ImGui::BeginTable("ProcessTable", 6, tableFlags, ImVec2(0, -40))) {
        ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableSetupColumn("Process Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("CPU (%)", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Memory (MB)", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("User", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < (int)m_processes.size(); ++i) {
            ImGui::TableNextRow();

            bool isSelected = (m_selectedProcessIndex == i);
            ImGui::TableSetColumnIndex(0);
            char label[32];
            snprintf(label, sizeof(label), "%d", m_processes[i].pid);

            if (ImGui::Selectable(label, isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                m_selectedProcessIndex = i;
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", m_processes[i].name.c_str());

            ImGui::TableSetColumnIndex(2);
            if (m_processes[i].cpuUsage > 20.0f) {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%.1f %%", m_processes[i].cpuUsage);
            } else {
                ImGui::Text("%.1f %%", m_processes[i].cpuUsage);
            }

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f MB", m_processes[i].memoryUsageMB);

            ImGui::TableSetColumnIndex(4);
            ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.4f, 1.0f), "%s", m_processes[i].status.c_str());

            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%s", m_processes[i].user.c_str());
        }

        ImGui::EndTable();
    }

    // Process Actions Bar
    ImGui::Separator();
    if (m_selectedProcessIndex >= 0 && m_selectedProcessIndex < (int)m_processes.size()) {
        ImGui::Text("Selected: %s (PID: %d)", m_processes[m_selectedProcessIndex].name.c_str(), m_processes[m_selectedProcessIndex].pid);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button("End Task", ImVec2(100, 24))) {
            m_processes.erase(m_processes.begin() + m_selectedProcessIndex);
            m_selectedProcessIndex = -1;
        }
        ImGui::PopStyleColor();
    } else {
        ImGui::TextDisabled("Select a process from the table to manage.");
    }
}

void TaskManager::RenderPerformanceTab() {
    ImGui::Text("System Resource Telemetry");
    ImGui::Separator();
    ImGui::Spacing();

    // CPU Plot
    float latestCpu = m_cpuHistory.back();
    char cpuOverlay[64];
    snprintf(cpuOverlay, sizeof(cpuOverlay), "CPU Usage: %.1f %%", latestCpu);
    ImGui::PlotLines("##CPUPlot", m_cpuHistory.data(), (int)m_cpuHistory.size(), 0, cpuOverlay, 0.0f, 100.0f, ImVec2(-1, 100));

    ImGui::Spacing();

    // RAM Plot
    float latestRam = m_ramHistory.back();
    char ramOverlay[64];
    snprintf(ramOverlay, sizeof(ramOverlay), "Memory Usage: %.1f %% (4.8 / 16.0 GB)", latestRam);
    ImGui::PlotLines("##RAMPlot", m_ramHistory.data(), (int)m_ramHistory.size(), 0, ramOverlay, 0.0f, 100.0f, ImVec2(-1, 100));
}

void TaskManager::RenderSystemSummaryTab() {
    ImGui::TextColored(ImVec4(0.38f, 0.69f, 0.98f, 1.0f), "System Hardware & OS Diagnostics");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BulletText("OS Version: CSOPESY Emulator OS v2.0 (x86_64)");
    ImGui::BulletText("Processor: Virtual Multi-Core CPU @ 3.60GHz");
    ImGui::BulletText("Total RAM: 16.0 GB DDR4");
    ImGui::BulletText("Graphics: OpenGL 3.0 via GLFW & ImGui Compositor");
    ImGui::BulletText("Architecture: Immediate-Mode Compositor Architecture");
    ImGui::BulletText("System Uptime: 0 days, 01:42:10");

    ImGui::Spacing();
    ImGui::Separator();
    if (ImGui::Button("Reset Process List", ImVec2(160, 28))) {
        InitializeProcesses();
    }
}

void TaskManager::UpdateAndRender(bool* pOpen) {
    if (!pOpen || !*pOpen) return;

    UpdateTelemetry(0.016f);

    ImGui::SetNextWindowSize(ImVec2(640, 420), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Task Manager - CSOPESY OS", pOpen, ImGuiWindowFlags_NoCollapse)) {
        if (ImGui::BeginTabBar("TaskManagerTabBar")) {
            if (ImGui::BeginTabItem("Processes")) {
                RenderProcessesTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Performance")) {
                RenderPerformanceTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("System Summary")) {
                RenderSystemSummaryTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
