# CSOPESY Semi-Major Output 2 (MO4) — Desktop-Style OS Mock-up

> **Group 19** — CSOPESY, De La Salle University

## Group Members

| Name | Role |
|---|---|
| Colcol, Llandro Massimo | Developer |
| Ong, Kyle Edward | Developer |
| Tayzon, Hannah Moriah | Developer |

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

## Feature Checklist

### Desktop
- Full-window **gradient wallpaper** (4 styles: `aurora`, `cyberpunk`, `midnight`, `sunset`) drawn on ImGui's background draw list.
- **Real-time clock** (12h or 24h) in the top-right corner, updated every frame.
- **[PWR] button** → confirmation dialog → graceful shutdown via `glfwSetWindowShouldClose`.
- **System Info widget** — togglable overlay showing system details.

### Taskbar (fixed, bottom)
Four drawn icon buttons with hover tooltips. A colored pill indicator beneath each icon shows whether the corresponding app is open:

| Icon | App | Description |
|---|---|---|
| 📁 Folder | **File Explorer** | Unique app screen 1 — browse a mock filesystem |
| 💻 Terminal | **CSOPESY Terminal** | Unique app screen 2 — interactive command prompt |
| 📊 Graph | **Task Manager** | Process table, performance graphs, system summary |
| ℹ️ Info | **System Info** | Toggle the system info widget overlay |

### Task Manager
- **Processes tab:** PID, process name, CPU %, memory (MB), status, and user. Dummy values fluctuate on a configurable timer to simulate a live system. Includes **End Task** button.
- **Performance tab:** Live CPU and memory usage graphs.
- **System Summary tab:** Overview of system resources.

### Terminal Commands
The CSOPESY Terminal (`AppScreen2`) supports the following commands:

| Command | Description |
|---|---|
| `help` | Display available commands |
| `ver` | Display OS kernel version |
| `ps` | List running processes |
| `top` | Display CPU and memory statistics |
| `clock` | Output current system time |
| `echo <text>` | Print text to the terminal |
| `clear` | Clear terminal buffer |
| `exit` | Close the terminal window |

---

## Runtime Configuration — `config.txt`

Edit `config.txt` in the project root, save, and relaunch. **No recompilation needed.** Missing keys use built-in defaults; invalid values are clamped or ignored.

### Window
| Key | Default | Description |
|---|---|---|
| `window_width` | `1280` | Window width in pixels |
| `window_height` | `720` | Window height in pixels |
| `window_title` | `CSOPESY Desktop OS Emulator` | Window title text |

### Boot Sequence
| Key | Default | Description |
|---|---|---|
| `boot_enabled` | `true` | `false` = skip straight to desktop |
| `bios_duration` | `6` | Seconds the BIOS/POST screen runs |
| `loading_duration` | `5` | Seconds the loading screen runs |
| `allow_skip` | `true` | Allow Space/Enter/click to skip boot screens |

### Desktop
| Key | Default | Description |
|---|---|---|
| `wallpaper` | `aurora` | `aurora` \| `cyberpunk` \| `midnight` \| `sunset` (or `0`–`3`) |
| `clock_24h` | `false` | `true` = 24-hour clock format |
| `show_system_widget` | `true` | Show the System Info widget at startup |

### Task Manager
| Key | Default | Description |
|---|---|---|
| `open_task_manager` | `true` | Open Task Manager window at startup |
| `telemetry_interval` | `0.5` | Seconds between CPU/memory telemetry updates |
| `total_ram_gb` | `16` | Total RAM shown in the Performance tab |
| `base_mem_used_mb` | `4500` | Base system memory usage (on top of process memory) |

### Dummy Processes
Add one line per process:
```
process = PID, Name, CPU%, MemoryMB, Status, User
```
Example:
```
process = 1004, csopesy_compositor.exe, 4.2, 128.5, Running, SYSTEM
process = 3412, csopesy_terminal.exe, 0.5, 32.1, Running, Student
```
Remove all `process` lines to use the built-in default process list.

---

## System Architecture

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
|   Config -----> loaded once from config.txt                 |
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

**Per-frame flow:**
```
glfwPollEvents -> ImGui NewFrame -> [Boot screens OR Desktop -> Taskbar -> Windows]
               -> shutdown check -> ImGui::Render -> OpenGL draw -> SwapBuffers
```

Components communicate through simple `bool*` flags held in `main.cpp` — no event bus or callback system. Clicking a taskbar icon toggles a flag; the corresponding window reads the same flag to show or hide itself.

---

## Folder Structure

```
CSOPESY-MO4/
├── src/                        # Our source code
│   ├── main.cpp                # Entry point, GLFW/ImGui setup, main loop
│   ├── Config.h / .cpp         # Parses config.txt into a Config struct
│   ├── Boot.h / .cpp           # BIOS/POST screen + loading screen state machine
│   ├── Desktop.h / .cpp        # Wallpaper, real-time clock, [PWR], System Info widget
│   ├── Taskbar.h / .cpp        # Icon dock with running-app indicators
│   ├── TaskManager.h / .cpp    # Process table, performance graphs, system summary
│   ├── AppScreen1.h / .cpp     # File Explorer (unique app screen 1)
│   └── AppScreen2.h / .cpp     # Terminal (unique app screen 2)
├── lib/
│   ├── imgui/                  # Dear ImGui (third-party, unmodified)
│   ├── include/                # GLFW headers
│   └── lib-vc2022/             # GLFW binaries (glfw3.dll)
├── .vscode/                    # VS Code build/run/debug configuration
├── build.ps1                   # PowerShell build script
├── build.bat                   # Batch wrapper for build.ps1
├── config.txt                  # Runtime parameters (no recompile needed)
├── PPT_Notes.md                # Technical report / presentation notes
├── MO4 - Desktop_GUI.docx      # Assignment specification
└── README.md                   # This file
```
