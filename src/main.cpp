#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Desktop.h"
#include "Taskbar.h"
#include "TaskManager.h"
#include "AppScreen1.h"
#include "AppScreen2.h"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char** argv) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return 1;
    }

    // OpenGL 3.0 / GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "CSOPESY OS Compositor - Desktop Shell Mockup", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window!\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Apply CSOPESY Modern Dark OS Styling
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;

    // Color Palette
    style.Colors[ImGuiCol_WindowBg]         = ImVec4(0.08f, 0.11f, 0.18f, 0.94f);
    style.Colors[ImGuiCol_Header]           = ImVec4(0.18f, 0.40f, 0.70f, 0.60f);
    style.Colors[ImGuiCol_HeaderHovered]    = ImVec4(0.24f, 0.50f, 0.85f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]     = ImVec4(0.28f, 0.58f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_Button]           = ImVec4(0.16f, 0.32f, 0.54f, 0.80f);
    style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.22f, 0.44f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]     = ImVec4(0.28f, 0.54f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_FrameBg]          = ImVec4(0.12f, 0.16f, 0.26f, 0.80f);
    style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.06f, 0.08f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.12f, 0.20f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_Tab]              = ImVec4(0.12f, 0.18f, 0.28f, 0.90f);
    style.Colors[ImGuiCol_TabHovered]       = ImVec4(0.24f, 0.45f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_TabActive]        = ImVec4(0.18f, 0.36f, 0.62f, 1.00f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Instantiate OS Subsystems
    Desktop desktop;
    Taskbar taskbar;
    TaskManager taskManager;
    AppScreen1 appScreen1; // File Explorer
    AppScreen2 appScreen2; // System Terminal

    // UI Window State Controls
    bool showTaskManager = true;
    bool showApp1 = false;
    bool showApp2 = false;
    bool showWidget = true;
    bool pwrShutdownRequested = false;

    // Main Compositor Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render Desktop Background Canvas & Top Bar with Clock & PWR shutdown button
        desktop.UpdateAndRender(&pwrShutdownRequested);

        // Render Taskbar Dock
        taskbar.UpdateAndRender(&showTaskManager, &showApp1, &showApp2, &showWidget);

        // Render Task Manager
        taskManager.UpdateAndRender(&showTaskManager);

        // Render Unique App Screen 1 (File Explorer)
        appScreen1.UpdateAndRender(&showApp1);

        // Render Unique App Screen 2 (System Terminal)
        appScreen2.UpdateAndRender(&showApp2);

        // Graceful PWR Shutdown Check
        if (pwrShutdownRequested) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Render Frame
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
