#include "UI.h"

#include <vector>
#include <string>

static const int MAX_FPS_HISTORY = 128;

static std::vector<float> fpsHistory = {0.0f};

static int counter = 0;

static ImVec4 m_ClearColor;
static ImVec4* m_StyleColors;
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

    Console::Initialize();
}

InspectorData UI::GetData()
{
    return s_Data;
}

float UI::getDeltaTime()
{
    return ImGui::GetIO().DeltaTime;
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

void UI::ShowConsole()
{
    Console::ShowConsole();
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
    ImGui::Begin("Inspector");

    ImGui::SeparatorText("Triangle");

    ImGui::BeginGroup();
    {
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", s_Data.m_Position, 0.02f, -1.0f, 1.0f);
        ImGui::DragFloat3("Rotation", s_Data.m_Rotation, 0.04f, 0.0f, 1.0f);
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
    float FPS = ImGui::GetIO().Framerate;
    if(fpsHistory.size() <= MAX_FPS_HISTORY)
    {
        fpsHistory.push_back(FPS);
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
                                        1000.0f / FPS, FPS);

    std::string deltaTime = "dTime:" + std::to_string(ImGui::GetIO().DeltaTime);
    ImGui::PlotLines("", fpsHistory.data(), (int)fpsHistory.size(), 0, deltaTime.c_str(), 0.0f, 1024, ImVec2(0, 80));

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

