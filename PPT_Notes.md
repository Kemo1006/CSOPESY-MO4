# CSOPESY MO4 Technical Report & Presentation Guide

This document contains technical details, architectural explanations, code snippets, and presentation content for the **CSOPESY Semi-Major Output 2 (MO4) PowerPoint Presentation (`.pptx`)**.

The PPT must contain: **Cover, Video Walkthrough, Architectural Diagram, Code Snippets, Written Discussion** (sections 1-4 below map to these).

---

## 1. System Architecture Overview (Architectural Diagram slide)

The CSOPESY OS Emulator is built as an **Immediate-Mode Compositor Shell** using C++, GLFW, OpenGL 3.0, and Dear ImGui.

```
+-------------------------------------------------------------+
|  Host OS (Windows)                                          |
|   GLFW -- window, input events, OpenGL 3.0 context          |
+--------------------------+----------------------------------+
                           | events / framebuffer
+--------------------------v----------------------------------+
|  Dear ImGui (immediate-mode UI) + OpenGL3 render backend    |
+--------------------------+----------------------------------+
                           | NewFrame -> widgets -> Render
+--------------------------v----------------------------------+
|  CSOPESY COMPOSITOR  (src/main.cpp, one loop, every frame)  |
|                                                             |
|   Config ----> loaded once from config.txt                  |
|                                                             |
|   BootSequence : BIOS -> Loading -> (done)                  |
|        | finished                                           |
|        v                                                    |
|   Layer 0  Desktop    wallpaper, clock, [PWR]    --+        |
|   Layer 1  Taskbar    icon dock (bottom)           | shared |
|   Layer 2  Windows    TaskManager                  | bool*  |
|                       AppScreen1 (File Explorer)   | flags  |
|                       AppScreen2 (Terminal)      --+        |
|                                                             |
|   [PWR] -> pwrShutdownRequested -> glfwSetWindowShouldClose |
+-------------------------------------------------------------+
```

**Per-frame flow**

```
glfwPollEvents -> ImGui NewFrame -> [Boot screens OR Desktop -> Taskbar -> Windows]
               -> shutdown check -> ImGui::Render -> OpenGL draw -> SwapBuffers
```

**Modules**

| File | Responsibility |
|---|---|
| `src/main.cpp` | Entry point. GLFW/ImGui setup, owns all window-open flags, runs the loop |
| `src/Config.cpp/.h` | Parses `config.txt` into one `Config` struct |
| `src/Boot.cpp/.h` | BIOS/POST screen, then loading screen |
| `src/Desktop.cpp/.h` | Wallpaper, real-time clock, [PWR] button, System Info widget |
| `src/Taskbar.cpp/.h` | Icon buttons and running-app indicators |
| `src/TaskManager.cpp/.h` | Process table, performance graphs, system summary |
| `src/AppScreen1.cpp/.h` | File Explorer (unique app screen 1) |
| `src/AppScreen2.cpp/.h` | Terminal (unique app screen 2) |

---

## 2. Component Design & Code Snippets (Code Snippets slides)

### A. Compositor Execution Loop (`src/main.cpp`)
The main loop processes input events, updates UI state, and renders every layer in order. The boot sequence runs first; the desktop starts when it finishes:

```cpp
while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (boot.UpdateAndRender()) {            // true once BIOS + loading are done
        desktop.UpdateAndRender(&pwrShutdownRequested, &showWidget);
        taskbar.UpdateAndRender(&showTaskManager, &showApp1, &showApp2, &showWidget);
        taskManager.UpdateAndRender(&showTaskManager);
        appScreen1.UpdateAndRender(&showApp1);
        appScreen2.UpdateAndRender(&showApp2);
    }
    if (pwrShutdownRequested) glfwSetWindowShouldClose(window, GLFW_TRUE);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}
```

### B. Desktop Wallpaper (`src/Desktop.cpp`)
The wallpaper is drawn with `ImDrawList` primitives on ImGui's **background draw list**, so it is always the bottom layer:

```cpp
RenderWallpaper(ImGui::GetBackgroundDrawList(), workSize.x, workSize.y);

drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(width, height),
                                  topColor, topColor, bottomColor, bottomColor);
```

### C. Real-Time Clock and PWR Shutdown (`src/Desktop.cpp`)

```cpp
time_t rawTime = time(NULL);
struct tm* timeInfo = localtime(&rawTime);
strftime(clockStr, sizeof(clockStr),
         Config::Get().clock24h ? "%A, %b %d, %Y | %H:%M:%S"
                                : "%A, %b %d, %Y | %I:%M:%S %p", timeInfo);

if (ImGui::Button(" [PWR] ", ImVec2(78.0f, 26.0f)))
    ImGui::OpenPopup("Confirm Shutdown");
if (ImGui::BeginPopupModal("Confirm Shutdown", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    if (ImGui::Button(" Shutdown ", ImVec2(120.0f, 30.0f))) {
        *pwrShutdownRequested = true;        // main loop then closes the window
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}
```

### D. Taskbar Icon Button with Running Indicator (`src/Taskbar.cpp`)

```cpp
if (ImGui::InvisibleButton(id, size) && windowState)
    *windowState = !*windowState;                  // click toggles the app's window

DrawIcon(dl, icon, center, color);                 // icon drawn with draw commands
if (isOpen)                                        // pill under every running app
    dl->AddRectFilled(ImVec2(pos.x + 14, max.y - 5), ImVec2(max.x - 14, max.y - 2),
                      ImGui::ColorConvertFloat4ToU32(accent), 2.0f);
```

### E. Task Manager Process Telemetry (`src/TaskManager.cpp`)
Processes are listed in an ImGui table; dummy CPU/memory values drift on a timer:

```cpp
if (ImGui::BeginTable("ProcessTable", 6, tableFlags, ImVec2(0, -40))) {
    ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 50.0f);
    ImGui::TableSetupColumn("Process Name", ImGuiTableColumnFlags_WidthFixed, 190.0f);
    ImGui::TableSetupColumn("CPU (%)", ImGuiTableColumnFlags_WidthFixed, 70.0f);
    ImGui::TableSetupColumn("Memory (MB)", ImGuiTableColumnFlags_WidthFixed, 90.0f);
    /* Status, User ... */
    ImGui::TableHeadersRow();
    for (size_t i = 0; i < m_processes.size(); ++i) { /* one row per process */ }
    ImGui::EndTable();
}

// every telemetry_interval seconds:
float cpuDelta = ((rand() % 100) - 50) / 20.0f;
m_processes[i].cpuUsage = std::max(0.1f, std::min(45.0f, m_processes[i].cpuUsage + cpuDelta));
```

### F. Runtime Parameters, No Recompiling (`src/Config.cpp`, `config.txt`)

```cpp
if (key == "wallpaper")          wallpaper = ParseWallpaper(val, wallpaper);
else if (key == "bios_duration") biosDuration = ParseFloat(val, biosDuration, 0.5f, 120.0f);
else if (key == "process")       { /* PID,Name,CPU,MemMB,Status,User -> ProcessSeed */ }
```

```
# config.txt
boot_enabled  = true
bios_duration = 6
wallpaper     = aurora
process = 1004, csopesy_compositor.exe, 4.2, 128.5, Running, SYSTEM
```

### G. Boot State Machine (`src/Boot.cpp`)

```cpp
enum Phase { PHASE_BIOS, PHASE_LOADING, PHASE_DONE };
...
if (advance || SkipRequested())
    m_phase = (m_phase == PHASE_BIOS) ? PHASE_LOADING : PHASE_DONE;
return m_phase == PHASE_DONE;      // main loop starts the desktop when true
```

---

## 3. Written Discussion: How We Designed the Desktop-Style OS

**Our approach:** We thought of the project as a tiny compositor. A real OS redraws its whole desktop from window state many times a second, and ImGui's immediate-mode style works the same way. Nothing is stored between frames. Each frame, our code says what should be on screen and ImGui draws it. That's why the whole emulator is one loop in `main.cpp` that calls one `UpdateAndRender` function per component.

**Layers:** The desktop is the bottom layer and gets drawn first. The wallpaper is a gradient with some animated shapes, all made with ImGui draw commands. The taskbar and the top bar sit above it as fixed windows, and the apps are normal movable windows on top of everything. Getting this order right took some trial and error. We first drew the wallpaper inside a full-screen window, and ImGui pushed it in front of the bars, so the clock, PWR button and taskbar disappeared. Switching to ImGui's background draw list fixed it, because that layer is always at the very bottom. 

**How  it works:** Our components don't call each other directly. `main.cpp` keeps a few simple flags (`showTaskManager`, `showApp1`, `showApp2`, `showWidget`, `pwrShutdownRequested`) and hands pointers to them. When you click a taskbar icon, it flips a flag, and the matching window sees the change and shows or hides itself. A window's own close button flips the same flag, so everything stays in sync without any event system. The taskbar uses the same flag to show which apps are running.

**Boot sequence:** Before the desktop appears, a small state machine plays a BIOS/POST screen and then a loading screen, inspired by the screenshots in the spec. It runs on real frame time, so the durations are accurate. Once it finishes, the main loop switches to the desktop. You can skip either screen with Space, Enter, or a click.

**Shutdown:** The spec says the app must be closed through the PWR button, so that's the only intended exit. Clicking it opens a confirmation dialog. Confirming sets a flag, and the main loop then asks GLFW to close the window and cleans up ImGui, OpenGL and GLFW in order. The program ends normally instead of being force-killed.

**Task Manager and apps:** The Task Manager is modeled on the Windows one. It has a Processes tab (PID, name, CPU, memory, status and user), a Performance tab with live CPU and memory graphs, and a System Summary tab. The numbers are dummy values that drift on a timer, so it feels like a running system. File Explorer and Terminal are our two unique screens, each with its own layout and placeholder content. The terminal even understands a few commands like `help`, `ps`, `top` and `clock`.

---