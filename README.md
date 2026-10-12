# CSOPESY (MO4) — Desktop-Style OS Mock-up

##  Group 19 Members

| Name | Role |
|---|---|
| Colcol, Llandro Massimo | Developer |
| Ong, Kyle Edward | Developer |
| Tayzon, Hannah Moriah | Developer |

## Submission Details

| Item | Detail |
|---|---|
| **Source code link** | https://github.com/Kemo1006/CSOPESY-MO4 |
| **Entry class file** | [`src/main.cpp`](src/main.cpp) — contains `main()` |
| **Group** | Group 19 (members listed above) |
| **README for graders** | `README.txt` (same instructions as this file) |

### Quick Run Instructions
1. Install **MSYS2 (UCRT64)** with `g++` and `gdb` at `C:\msys64`, plus **VS Code** with the Microsoft **C/C++** extension (see [Requirements](#requirements)).
2. Open the **CSOPESY-MO4** folder in VS Code (the folder that contains `.vscode/`).
3. Open `src/main.cpp` and press **F5** (`Debug CSOPESY`). It builds and launches the emulator.
4. Close the program with the **[PWR]** button (top-right) → **Shutdown**.

To change parameters (boot times, wallpaper, dummy processes, etc.), edit `config.txt` and relaunch. No recompiling is needed. See [Runtime Configuration](#runtime-configuration--configtxt).

## Entry Point

[`src/main.cpp`](src/main.cpp) — contains `main()`.

## Technology Stack

| Component | Version / Detail |
|---|---|
| Language | C++17 |
| UI Framework | Dear ImGui 1.93 (immediate-mode) |
| Windowing | GLFW 3.4 |
| Graphics API | OpenGL 3.0 / GLSL 130 |
| Compiler | MSYS2 UCRT64 GCC (MinGW-w64) |

---

## Requirements

- **Windows 10 / 11**
- **MSYS2** with the UCRT64 toolchain installed at `C:\msys64`
  ```
  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
  ```
- **Visual Studio Code** + Microsoft **C/C++** extension (recommended, not required for terminal builds)

---

## How to Build and Run

### VS Code (recommended)
1. Open the **CSOPESY-MO4** folder in VS Code (the folder containing `.vscode/`).
2. Open `src/main.cpp` and press **F5** (`Debug CSOPESY`) — builds and launches.
3. **Build only:** `Ctrl+Shift+B`

### Terminal (from project root)
```powershell
.\build.ps1          # build + run
.\build.ps1 -NoRun   # build only
```
Or double-click **`build.bat`**.

### Output
Compiled executable: `build/main.exe` (the build script copies `glfw3.dll` next to it automatically).

> **Note:** The build script automatically prioritizes the MSYS2 UCRT64 `g++` when available. If you have other MinGW/Dev-C++ compilers on `PATH`, they will not interfere.

### Shutting Down
The program **must** be closed using the **[PWR]** button (top-right corner of the desktop) → **Shutdown**. This triggers a graceful cleanup of ImGui, OpenGL, and GLFW resources.

---

## What Happens When It Starts

The emulator boots through three phases:

```
BIOS / POST Screen  →  CSOPESY Loading Screen  →  Desktop
```

- **BIOS screen** — simulated POST output, runs for `bios_duration` seconds (default: 6).
- **Loading screen** — progress bar, runs for `loading_duration` seconds (default: 5).
- **Skip:** Press `Space`, `Enter`, or click to skip the current boot screen.
- Set `boot_enabled = false` in `config.txt` to go straight to the desktop.

---

## Runtime Configuration — `config.txt`

Edit `config.txt` in the project root, save, and relaunch. **No recompilation needed.** Missing keys use built-in defaults; invalid values are clamped or ignored.

---