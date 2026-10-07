#include "AppScreen1.h"

AppScreen1::AppScreen1() 
    : m_currentPath("/root/system_volume"), m_selectedFileIndex(-1) {
    LoadPath(m_currentPath);
}

AppScreen1::~AppScreen1() {
}

void AppScreen1::LoadPath(const std::string& path) {
    m_currentPath = path;
    m_files.clear();
    m_selectedFileIndex = -1;

    if (path == "/root/system_volume") {
        m_files.push_back({ "Documents", "Folder", "--", "2026-10-01", true });
        m_files.push_back({ "Downloads", "Folder", "--", "2026-10-05", true });
        m_files.push_back({ "System32", "System Folder", "--", "2026-09-18", true });
        m_files.push_back({ "kernel.sys", "System File", "4,096 KB", "2026-09-15", false });
        m_files.push_back({ "config.ini", "Configuration", "12 KB", "2026-10-06", false });
        m_files.push_back({ "csopesy_os.log", "Text Document", "128 KB", "2026-10-07", false });
    } else if (path == "/root/system_volume/Documents") {
        m_files.push_back({ "..", "Parent Directory", "--", "--", true });
        m_files.push_back({ "MO4_Requirements.pdf", "PDF Document", "1,058 KB", "2026-09-18", false });
        m_files.push_back({ "Project_Report.docx", "Word Document", "2,450 KB", "2026-10-04", false });
        m_files.push_back({ "Architectural_Diagram.png", "PNG Image", "890 KB", "2026-10-06", false });
    } else {
        m_files.push_back({ "..", "Parent Directory", "--", "--", true });
        m_files.push_back({ "placeholder_file.dat", "Data File", "64 KB", "2026-10-07", false });
    }
}

void AppScreen1::UpdateAndRender(bool* pOpen) {
    if (!pOpen || !*pOpen) return;

    ImGui::SetNextWindowSize(ImVec2(600, 380), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("File Explorer - CSOPESY OS", pOpen, ImGuiWindowFlags_NoCollapse)) {
        // Navigation bar
        ImGui::Text("Location:");
        ImGui::SameLine();
        ImGui::PushItemWidth(-120.0f);
        char pathBuf[256];
        snprintf(pathBuf, sizeof(pathBuf), "%s", m_currentPath.c_str());
        ImGui::InputText("##PathInput", pathBuf, sizeof(pathBuf), ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Up [..]", ImVec2(100.0f, 0))) {
            if (m_currentPath != "/root/system_volume") {
                LoadPath("/root/system_volume");
            }
        }

        ImGui::Separator();

        // Left sidebar (Quick Access) + Right main file view
        ImGui::BeginChild("QuickAccessSidebar", ImVec2(150, 0), true);
        ImGui::TextColored(ImVec4(0.38f, 0.69f, 0.98f, 1.0f), "Quick Access");
        ImGui::Separator();
        if (ImGui::Selectable("Root Volume", m_currentPath == "/root/system_volume")) {
            LoadPath("/root/system_volume");
        }
        if (ImGui::Selectable("Documents", m_currentPath == "/root/system_volume/Documents")) {
            LoadPath("/root/system_volume/Documents");
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("Storage Drive:");
        ImGui::Text("CSOPESY-C: 45%%");
        ImGui::ProgressBar(0.45f, ImVec2(-1, 14), "112GB free");
        ImGui::EndChild();

        ImGui::SameLine();

        // Main Files Table
        ImGui::BeginChild("FileGridArea", ImVec2(0, 0), true);
        if (ImGui::BeginTable("FilesTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 110.0f);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("Date Modified", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            for (int i = 0; i < (int)m_files.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                bool isSelected = (m_selectedFileIndex == i);
                std::string icon = m_files[i].isDirectory ? "[DIR]" : "[FILE]";
                char label[128];
                snprintf(label, sizeof(label), "%s %s", icon.c_str(), m_files[i].name.c_str());

                if (ImGui::Selectable(label, isSelected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                    m_selectedFileIndex = i;
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        if (m_files[i].isDirectory) {
                            if (m_files[i].name == "..") {
                                LoadPath("/root/system_volume");
                            } else {
                                LoadPath(m_currentPath + "/" + m_files[i].name);
                            }
                        }
                    }
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", m_files[i].type.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", m_files[i].size.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", m_files[i].dateModified.c_str());
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
