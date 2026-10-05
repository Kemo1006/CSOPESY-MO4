CSOPESY MO4 - Desktop-Style OS Mock-up
======================================

Group 19 Members
-------------
- Colcol, Llandro Massimo
- Ong, Kyle Edward
- Tayzon, Hannah Moriah

Entry Point
-----------
src/main.cpp  (contains main())

Requirements
------------
- Windows 10/11
- MSYS2 with UCRT64 g++ (C:\msys64\ucrt64\bin on PATH)
    pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
- Visual Studio Code + Microsoft "C/C++" extension

How to Build and Run
--------------------
VS Code:
  - Run/Debug: press F5 ("Debug CSOPESY") - builds then launches
  - Build only: Ctrl+Shift+B
Terminal (from project root):
  .\build.ps1          build + run
  .\build.ps1 -NoRun   build only
Output: build\main.exe (glfw3.dll is copied next to it automatically)

Folder Structure
----------------
src/            our source code
lib/imgui/      Dear ImGui (third-party, unmodified)
lib/include/    GLFW headers
lib/lib-vc2022/ GLFW binaries
.vscode/        build/run/debug configuration
build.ps1       build script
