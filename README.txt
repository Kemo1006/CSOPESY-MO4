========================================================================
  CSOPESY SEMI-MAJOR OUTPUT 2 (MO4) - DESKTOP-STYLE OS MOCK-UP
  Group 19
========================================================================

GROUP MEMBERS
  - Colcol, Llandro Massimo
  - Ong, Kyle Edward
  - Tayzon, Hannah Moriah

------------------------------------------------------------------------
SUBMISSION DETAILS
------------------------------------------------------------------------
  Source code link :  https://github.com/Kemo1006/CSOPESY-MO4
  Entry class file :  src/main.cpp   (contains main())
  Group            :  Group 19 (members listed above)

  QUICK RUN INSTRUCTIONS
    1. Install MSYS2 (UCRT64) with g++ and gdb at C:\msys64, plus VS Code
       with the Microsoft "C/C++" extension (see REQUIREMENTS below).
    2. Open the CSOPESY-MO4 folder in VS Code (the folder that contains .vscode).
    3. Open src/main.cpp and press F5 ("Debug CSOPESY").
       It builds and launches the emulator.
    4. Close the program with the [PWR] button (top-right) -> "Shutdown".

  To change parameters (boot times, wallpaper, dummy processes, etc.),
  edit config.txt and relaunch. No recompiling is needed.
  See RUNTIME PARAMETERS below.

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