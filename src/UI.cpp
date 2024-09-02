#include "UI.h"

#include <vector>
#include <algorithm> // для std::min и std::max

static const int MAX_FPS_HISTORY = 128;

static std::vector<float> fpsHistory = {0.0f};

static int counter = 0;

static ImVec4 m_ClearColor;
static ImVec4* m_StyleColors;
static std::string m_Log;
InspectorData UI::s_Data;

void UI::Init(GLFWwindow* window)
{
    m_ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.FrameRounding = 4;
    style.GrabRounding = 4;
    style.WindowRounding = 6;
    m_StyleColors = style.Colors;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

InspectorData UI::GetData()
{
    return s_Data;
}

void UI::Run()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_StyleColors[ImGuiCol_DockingEmptyBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
}

void UI::Render()
{
    ShowDocker();
    ShowScene();
    ShowConsole();
    ShowHierarchy();
    ShowInspector();
    ShowProject();
    ShowMonitorPerformance();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::LogMessage(const std::string& message)
{
    m_Log += message + '\n';
}

void UI::ShowConsole()
{
    ImGui::Begin("Console");

    if(ImGui::Button("Clear"))
    {
        m_Log.clear();
    }

    ImGui::SameLine();

    if(ImGui::Button("g"))
    {
        LogMessage(std::to_string(ImGui::GetTime()));
    }

    ImGui::SameLine();

    if(ImGui::Button("Time"))
    {
        LogMessage(std::to_string(ImGui::GetTime()));
    }

    ImGui::SeparatorText("Log");
    ImGui::BeginChild("Log");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", m_Log.c_str());
    ImGui::EndChild();
    ImGui::End();
}

void UI::ShowHierarchy()
{
    ImGui::Begin("Hierarchy");

    if(ImGui::CollapsingHeader("Triangle"))
    {
        ImGui::Text("Position: (%f, %f, %f)", s_Data.m_Position[0], s_Data.m_Position[1], s_Data.m_Position[2]);
        ImGui::Text("Rotation: (%f, %f, %f)", s_Data.m_Rotation[0], s_Data.m_Rotation[1], s_Data.m_Rotation[2]);
        ImGui::Text("Scale: (%f, %f, %f)", s_Data.m_Scale[0], s_Data.m_Scale[1], s_Data.m_Scale[2]);
    }

    ImGui::End();
}

void UI::ShowInspector()
{
    s_Data.m_Position[0] = s_Data.m_Position[1] = s_Data.m_Position[2] = 0.0f;
    s_Data.m_Rotation[0] = s_Data.m_Rotation[1] = s_Data.m_Rotation[2] = 0.0f;
    s_Data.m_Scale[0] = s_Data.m_Scale[1] = s_Data.m_Scale[2] = 1.0f;
    ImGui::Begin("Inspector");

    ImGui::SeparatorText("Triangle");

    ImGui::BeginGroup();
    {
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", s_Data.m_Position, 0.02f, -1.0f, 1.0f);
        ImGui::DragFloat3("Rotation", s_Data.m_Rotation, 0.04f, -1.0f, 1.0f);
        ImGui::DragFloat3("Scale", s_Data.m_Scale, 0.01f, 1.0f, 1.0f);
    }
    ImGui::EndGroup();

    ImGui::NewLine();

    ImGui::BeginGroup();
    ImGui::Text("Colors");
    ImGui::ColorEdit3("Shader Color", s_Data.m_Color);
    ImGui::ColorEdit3("Clear Color", (float*)&m_ClearColor);
    ImGui::EndGroup();

    ImGui::End();
}


void UI::ShowMonitorPerformance()
{
    if(fpsHistory.size() <= MAX_FPS_HISTORY)
    {
        fpsHistory.push_back(ImGui::GetIO().Framerate);
    }
    if (counter >= 20)
    {
        if (fpsHistory.size() > MAX_FPS_HISTORY)
        {
            fpsHistory.erase(fpsHistory.begin());
        }
        counter = 0;
    }
    counter++;

    ImGui::Begin("Monitor Performance");
    ImGui::Text("DEVICE : %s", glGetString(GL_RENDERER));
    ImGui::Text("OPEN_GL : %.18s", glGetString(GL_VERSION));
    ImGui::Text("GLSL : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    ImGui::TextColored(ImVec4(0,1,0,1), "Application average %.3f ms/frame (%.1f FPS)",
                                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::PlotLines("", fpsHistory.data(), fpsHistory.size(), 0, "max: 1024graph", 0.0f, 1024.0f, ImVec2(0, 80));

    ImGui::SeparatorText("Framerate mode");

    if(ImGui::Button("no_limit"))
        glfwSwapInterval(0);
    ImGui::SameLine();
    if(ImGui::Button("60_limit"))
        glfwSwapInterval(1);
    ImGui::SameLine();
    if(ImGui::Button("30_limit"))
        glfwSwapInterval(2);
    ImGui::End();
}

void UI::ShowDocker()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpace Window", nullptr, window_flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::End();
}

void UI::ShowScene()
{
    ImGui::Begin("Scene");
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)FrameBuffer::texture_id, size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void UI::ShowProject()
{
    ImGui::Begin("Project");

    ImGui::End();
}

