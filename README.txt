========================================================================
                      CSOPESY SEMI-MAJOR OUTPUT 2 (MO4)
                  DESKTOP-STYLE OPERATING SYSTEM MOCKUP
========================================================================

PROJECT INFORMATION:
- Course: CSOPESY (Operating Systems)
- Project: Semi-Major Output 2 (MO4) - Desktop-Style OS Mockup
- Framework: C++11, Dear ImGui, GLFW 3.4, OpenGL 3.0
- Entry File / Main Function: src/main.cpp (main function)

DEVELOPER CREDENTIALS:
- Prepared By: Gregory Cu
- Created By: Neil Patrick Del Gallego, PhD

------------------------------------------------------------------------
CHECKLIST OF IMPLEMENTED REQUIREMENTS:
------------------------------------------------------------------------
[X] DESKTOP BACKGROUND:
    - Base layer rendered first each frame across the full viewport.
    - Customizable wallpapers: Deep Aurora gradient & animation, Cyberpunk 
      Grid, Midnight Blue, and Sunset Horizon.
    - Real-time digital clock (HH:MM:SS AM/PM) and date display updated 
      every frame in the top system bar.
    - Dedicated [PWR] Power button in the system bar for graceful 
      application shutdown with modal confirmation (no force exit required).

[X] TASKBAR DOCK:
    - Anchored fixed panel at the bottom of the screen.
    - Interactive icon launcher buttons for system applications:
      1. [TASK] Task Manager: Opens/toggles Windows-style Task Manager.
      2. [DIR]  File Explorer: Opens/toggles Unique App Screen 1.
      3. [CMD]  Terminal: Opens/toggles Unique App Screen 2.
      4. [SYS]  System Info: Toggles Desktop System Widget.

[X] TASK MANAGER:
    - Replicates Windows Task Manager UI design with tabbed navigation:
      1. Processes Tab: Table listing active PIDs, Process Names, CPU (%), 
         Memory Usage (MB), Status, and User. Supports selecting processes 
         and simulated "End Task".
      2. Performance Tab: Dynamic real-time telemetry line graphs for CPU 
         and Memory utilization.
      3. System Summary Tab: Hardware diagnostic details and system uptime.
    - Uses live updated dummy telemetry values for a realistic feel.

[X] UNIQUE APP SCREENS:
    - App Screen 1 (File Explorer): Directory navigation tree, file grid, 
      quick access sidebar, and storage drive space meter.
    - App Screen 2 (CSOPESY Terminal): Interactive shell emulator supporting 
      commands ('help', 'ver', 'ps', 'top', 'clock', 'clear', 'exit').

------------------------------------------------------------------------
BUILD & EXECUTION INSTRUCTIONS:
------------------------------------------------------------------------
Option A: Running Pre-built Executable
  1. Open a Command Prompt or Terminal in this folder.
  2. Run the generated binary:
     CSOPESY_DesktopOS.exe

Option B: Recompiling from Source (MinGW / GCC)
  1. Ensure MinGW g++ is installed and accessible (e.g. TDM-GCC or MinGW64).
  2. Double-click or execute 'build.bat' in the command line:
     .\build.bat
  3. The build script automatically compiles all C++ and C sources in 'src/', 
     'vendor/imgui', and 'vendor/glfw' and links OpenGL libraries.
  4. Upon build completion, launch:
     CSOPESY_DesktopOS.exe

------------------------------------------------------------------------
PROJECT STRUCTURE:
------------------------------------------------------------------------
  ├── src/
  │   ├── main.cpp         <-- ENTRY POINT (main function)
  │   ├── Desktop.h/.cpp   <-- Desktop canvas, wallpaper, live clock, PWR shutdown
  │   ├── Taskbar.h/.cpp   <-- Fixed bottom dock & app launchers
  │   ├── TaskManager.h/.cpp <-- Windows-style Task Manager & telemetry
  │   ├── AppScreen1.h/.cpp <-- File Explorer application screen
  │   └── AppScreen2.h/.cpp <-- Interactive Terminal application screen
  ├── vendor/
  │   ├── imgui/           <-- Dear ImGui source & backends
  │   └── glfw/            <-- GLFW source & headers
  ├── build.bat            <-- Compilation build script
  ├── README.txt           <-- Documentation & instructions
  └── PPT_Notes.md         <-- Presentation report guidelines & code snippets
========================================================================
