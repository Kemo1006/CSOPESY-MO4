========================================================================
  CSOPESY SEMI-MAJOR OUTPUT 2 (MO4) - DESKTOP-STYLE OS MOCK-UP
  Group 19
========================================================================

GROUP MEMBERS
  - Colcol, Llandro Massimo
  - Ong, Kyle Edward
  - Tayzon, Hannah Moriah

ENTRY POINT
  src/main.cpp   (contains main())

STACK
  C++17, Dear ImGui 1.93, GLFW 3.4, OpenGL 3.0 (GLFW + OpenGL + ImGui compositor)

------------------------------------------------------------------------
REQUIREMENTS
------------------------------------------------------------------------
  - Windows 10/11
  - MSYS2 (UCRT64) with g++ and gdb, installed at C:\msys64
        pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
  - Visual Studio Code + Microsoft "C/C++" extension

------------------------------------------------------------------------
HOW TO RUN
------------------------------------------------------------------------
  1. Open the CSOPESY-MO4 folder in VS Code (the folder that contains .vscode).
  2. Open src/main.cpp and press F5 ("Debug CSOPESY").
     It compiles everything, then launches the emulator.
  Build only:  Ctrl+Shift+B
  Terminal alternative (project root):  .\build.ps1   or   build.bat

  The program must be closed with the [PWR] button (top-right), then "Shutdown".

------------------------------------------------------------------------
WHAT HAPPENS WHEN IT STARTS
------------------------------------------------------------------------
  1. BIOS / POST screen      (Space, Enter or a click skips it)
  2. CSOPESY loading screen  (Space, Enter or a click skips it)
  3. Desktop

------------------------------------------------------------------------
REQUIREMENT CHECKLIST
------------------------------------------------------------------------
  DESKTOP
    - Full-window gradient wallpaper (4 styles) drawn first each frame on
      ImGui's background draw list.
    - Real-time clock (updated every frame), top-right corner.
    - [PWR] button -> confirm dialog -> graceful shutdown.
  TASKBAR (fixed, bottom)
    - 4 drawn icon buttons with hover tooltips; a pill under each icon
      shows that the app is running:
        Folder   = File Explorer (unique app screen 1)
        Terminal = CSOPESY Terminal (unique app screen 2)
        Graph    = Task Manager
        "i"      = System Info widget (toggle)
  TASK MANAGER
    - Processes tab: PID / name / CPU % / memory / status / user, dummy
      values that fluctuate, End Task. Performance tab: live CPU and
      memory graphs. System Summary tab.

------------------------------------------------------------------------
RUNTIME PARAMETERS - config.txt (no recompiling needed)
------------------------------------------------------------------------
  Edit config.txt in the project root, save, and relaunch. Supported:
    window_width / window_height / window_title
    boot_enabled, bios_duration, loading_duration, allow_skip
    wallpaper (aurora|cyberpunk|midnight|sunset), clock_24h
    show_system_widget, open_task_manager
    telemetry_interval, total_ram_gb, base_mem_used_mb
    process = PID, Name, CPU, MemMB, Status, User   (one line per process)
  Missing keys use defaults; bad values are clamped/ignored.
  (Terminal commands: help, ver, ps, top, clock, echo, clear, exit)

------------------------------------------------------------------------
FOLDER STRUCTURE
------------------------------------------------------------------------
  src/                our source code
    main.cpp          entry point, GLFW/ImGui setup, main loop
    Config.h/.cpp     loads config.txt
    Boot.h/.cpp       BIOS + loading screens
    Desktop.h/.cpp    wallpaper, clock, PWR, System Info widget
    Taskbar.h/.cpp    icon dock
    TaskManager.h/.cpp
    AppScreen1.h/.cpp File Explorer
    AppScreen2.h/.cpp Terminal
  lib/imgui/          Dear ImGui (third-party, unmodified)
  lib/include, lib/lib-vc2022   GLFW headers and binaries
  config.txt          runtime parameters
  .vscode/            F5 / build configuration
  build.ps1, build.bat   build scripts
  PPT_Notes.md        notes for the technical report
========================================================================
