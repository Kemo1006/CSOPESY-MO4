@echo off
echo ===================================================
echo Building CSOPESY Desktop OS Mockup Executable...
echo ===================================================

set GCC_PATH="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe"
set GXX_PATH="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe"

if not exist %GCC_PATH% (
    set GCC_PATH=gcc
    set GXX_PATH=g++
)

echo [1/3] Compiling GLFW C Backend...
%GCC_PATH% -c -O2 -D_GLFW_WIN32 -D_WIN32_WINNT=0x0501 -D"MSGFLT_ALLOW=1" -D"ChangeWindowMessageFilterEx(a,b,c,d)=TRUE" -D"SetProcessDPIAware()=TRUE" ^
    -Ivendor/glfw/include -Ivendor/glfw/src ^
    vendor/glfw/src/context.c ^
    vendor/glfw/src/init.c ^
    vendor/glfw/src/input.c ^
    vendor/glfw/src/monitor.c ^
    vendor/glfw/src/platform.c ^
    vendor/glfw/src/vulkan.c ^
    vendor/glfw/src/window.c ^
    vendor/glfw/src/egl_context.c ^
    vendor/glfw/src/osmesa_context.c ^
    vendor/glfw/src/null_init.c ^
    vendor/glfw/src/null_monitor.c ^
    vendor/glfw/src/null_window.c ^
    vendor/glfw/src/null_joystick.c ^
    vendor/glfw/src/win32_init.c ^
    vendor/glfw/src/win32_joystick.c ^
    vendor/glfw/src/win32_module.c ^
    vendor/glfw/src/win32_monitor.c ^
    vendor/glfw/src/win32_thread.c ^
    vendor/glfw/src/win32_time.c ^
    vendor/glfw/src/win32_window.c ^
    vendor/glfw/src/wgl_context.c

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: GLFW compilation failed!
    exit /b %ERRORLEVEL%
)

echo [2/3] Compiling Dear ImGui and CSOPESY OS C++ Engine...
%GXX_PATH% -c -O2 -std=c++11 "-Dlocaltime_s(a,b)=(*a=*localtime(b),0)" -D_GLFW_WIN32 ^
    -Ivendor/imgui ^
    -Ivendor/imgui/backends ^
    -Ivendor/glfw/include ^
    -Ivendor/glfw/src ^
    -Isrc ^
    src/main.cpp ^
    src/Desktop.cpp ^
    src/Taskbar.cpp ^
    src/TaskManager.cpp ^
    src/AppScreen1.cpp ^
    src/AppScreen2.cpp ^
    vendor/imgui/imgui.cpp ^
    vendor/imgui/imgui_draw.cpp ^
    vendor/imgui/imgui_tables.cpp ^
    vendor/imgui/imgui_widgets.cpp ^
    vendor/imgui/imgui_demo.cpp ^
    vendor/imgui/backends/imgui_impl_glfw.cpp ^
    vendor/imgui/backends/imgui_impl_opengl3.cpp

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: C++ Engine compilation failed!
    exit /b %ERRORLEVEL%
)

echo [3/3] Linking Final Executable CSOPESY_DesktopOS.exe...
%GXX_PATH% main.o Desktop.o Taskbar.o TaskManager.o AppScreen1.o AppScreen2.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_demo.o imgui_impl_glfw.o imgui_impl_opengl3.o context.o init.o input.o monitor.o platform.o vulkan.o window.o egl_context.o osmesa_context.o null_init.o null_monitor.o null_window.o null_joystick.o win32_init.o win32_joystick.o win32_module.o win32_monitor.o win32_thread.o win32_time.o win32_window.o wgl_context.o -lopengl32 -lgdi32 -luser32 -lshell32 -ldwmapi -lwinmm -limm32 -o CSOPESY_DesktopOS.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===================================================
    echo BUILD SUCCESSFUL!
    echo Executable generated: CSOPESY_DesktopOS.exe
    echo ===================================================
    del *.o 2>NUL
) else (
    echo.
    echo ===================================================
    echo BUILD FAILED! Check error messages above.
    echo ===================================================
)
