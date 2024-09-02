#ifndef UI_H
#define UI_H

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "graphics/FrameBuffer.h"

#include <glfw/glfw3.h>
#include <string>

using uint = unsigned int;

struct InspectorData
{
    float m_Position[3] = {0.0f, 0.0f, 0.0f};
    float m_Rotation[3] = {0.0f, 0.0f, 0.0f};
    float m_Scale[3] = {1.0f, 1.0f, 1.0f};
    float m_Color[3] = {1.0f, 1.0f, 1.0f};
};

class UI
{
public:

    static void Init(GLFWwindow* window);
    static void Run();
    static void Render();
    static void Shutdown();

    static void LogMessage(const std::string& message);
    static InspectorData GetData();

    static void ShowDocker();
    static void ShowScene();
    static void ShowHierarchy();
    static void ShowProject();
    static void ShowConsole();
    static void ShowInspector();
    static void ShowMonitorPerformance();

private:
    static InspectorData s_Data;
};

#endif // UI_H