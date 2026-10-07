# CSOPESY MO4 Technical Report & Presentation Guide

This document contains technical details, architectural explanations, code snippets, and presentation content for the **CSOPESY Semi-Major Output 2 (MO4) PowerPoint Presentation (`.pptx`)**.

---

## 1. System Architecture Overview

The CSOPESY OS Emulator is built as an **Immediate-Mode Compositor Shell** using C++, GLFW, OpenGL 3.0, and Dear ImGui.

```
+-------------------------------------------------------------------+
|                        CSOPESY OS Compositor                      |
|                           (src/main.cpp)                          |
+-------------------------------------------------------------------+
                                  |
         +------------------------+------------------------+
         |                        |                        |
+------------------+     +------------------+     +------------------+
| Desktop Engine   |     |  Taskbar Dock    |     | Application Layer|
| (Desktop.cpp)    |     |  (Taskbar.cpp)   |     +------------------+
+------------------+     +------------------+              |
| - Wallpaper      |     | - App Launchers  |   +----------+----------+
| - Real-time Clock|     | - Window Toggles |   |                     |
| - PWR Shutdown   |     +------------------+   v                     v
+------------------+                     +--------------+     +---------------+
                                         | Task Manager |     | Custom Apps   |
                                         | (Processes & |     | - File Explorer|
                                         |  Telemetry)  |     | - Terminal    |
                                         +--------------+     +---------------+
```

---

## 2. Component Design & Code Snippets

### A. Compositor Execution Loop (`src/main.cpp`)
The main loop processes input events, manages frame timing via GLFW vsync, updates UI states, and renders every layer in order:

```cpp
// Main Compositor Loop
while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Render Desktop Background Canvas & Top Bar
    desktop.UpdateAndRender(&pwrShutdownRequested);

    // 2. Render Taskbar Dock
    taskbar.UpdateAndRender(&showTaskManager, &showApp1, &showApp2, &showWidget);

    // 3. Render Task Manager & Apps
    taskManager.UpdateAndRender(&showTaskManager);
    appScreen1.UpdateAndRender(&showApp1);
    appScreen2.UpdateAndRender(&showApp2);

    // 4. Graceful Power Shutdown Check
    if (pwrShutdownRequested) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}
```

### B. Desktop Wallpaper & Real-Time Clock (`src/Desktop.cpp`)
Wallpapers are generated using `ImDrawList` primitives (gradients, glowing circles, grid lines), and system time is formatted per frame:

```cpp
// Real-time clock format
time_t rawTime = time(NULL);
struct tm* timeInfo = localtime(&rawTime);
char clockStr[64], dateStr[64];
strftime(clockStr, sizeof(clockStr), "%I:%M:%S %p", timeInfo);
strftime(dateStr, sizeof(dateStr), "%b %d, %Y", timeInfo);
ImGui::TextColored(ImVec4(0.9f, 0.95f, 1.0f, 1.0f), "%s  |  %s", dateStr, clockStr);
```

### C. Task Manager Process Telemetry (`src/TaskManager.cpp`)
Processes are listed in a sortable ImGui table with simulated CPU/Memory telemetry:

```cpp
if (ImGui::BeginTable("ProcessTable", 6, tableFlags)) {
    // Column headers: PID, Name, CPU, Memory, Status, User
    for (int i = 0; i < m_processes.size(); ++i) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Selectable(label, isSelected);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.1f %%", m_processes[i].cpuUsage);
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("%.1f MB", m_processes[i].memoryUsageMB);
    }
    ImGui::EndTable();
}
```

---

## 3. Video Walkthrough Recording Instructions
When recording your video walkthrough for the presentation:
1. Show your IDE (Dev-C++ / VS Code / Terminal).
2. Press **Run** or execute `.\CSOPESY_DesktopOS.exe`.
3. Demonstrate:
   - **Desktop Background**: Change wallpaper theme using the desktop widget.
   - **Real-Time Clock**: Point out the live updating seconds in the top bar.
   - **Taskbar**: Click taskbar icons to show/hide Task Manager, File Explorer, and Terminal.
   - **Task Manager**: Show the processes table updating dynamically and test selecting a process to click "End Task".
   - **Terminal Screen**: Type `help`, `ver`, `ps`, and `top`.
   - **PWR Shutdown**: Click `[PWR]` in the top-right bar and confirm shutdown.
4. Embed the `.mp4` file directly into your PowerPoint `.pptx` slides.
