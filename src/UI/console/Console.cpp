#include "Console.h"

#define TAB "\t"

char Console::m_InputBuffer[255];

std::vector<LogEntry> Console::m_LogEntries;

static CommandManager commandManager;

void Console::Initialize()
{
    commandManager.RegisterCommand("clear", "Clears the console.", [](const std::vector<std::string>& args) {
        clearConsole();
    });

    commandManager.RegisterCommand("time", "Shows the current program time.", [](const std::vector<std::string>& args) {
        std::string message = TAB + std::to_string(ImGui::GetTime());
        LogMessage(message, LogType::LOG);
    });

    commandManager.RegisterCommand("help", "Shows all available commands.", [](const std::vector<std::string>& args) {
        std::string message;
        for(const auto& command : commandManager.commands)
        {
            message += "\t" + command.Name + "\n\t - " + command.Description + "\n\n";
        }
        LogMessage(message, LogType::LOG);
    });
}

void Console::ShowConsole()
{
    ImGui::Begin("Console");
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, ImGui::GetContentRegionAvail().y - 25), true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

    // DRAW TEXT (UNFORMATTED)
    Render();


    // Автопрокрутка к низу
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);


    ImGui::PopStyleVar();
    ImGui::EndChild();

    // INPUT TEXT
    InputText();


    ImGui::End();
}

void Console::LogMessage(const std::string &message, LogType type)
{
    m_LogEntries.push_back({message, type});
}

void Console::Render()
{
    // DRAW TEXT (UNFORMATTED)
    for(const auto &entry : m_LogEntries)
    {
        ImVec4 textColor;
        switch (entry.type)
        {
        case COMMAND :
            textColor = ImVec4(1.f, 1.f, 1.f, 1.f); // white
            break;

        case LOG :
            textColor = ImVec4(1.f, 1.f, 1.f, 0.5f); // Grey
            break;

        case WARRNING :
            textColor = ImVec4(1.0f, 0.87f, 0.37f, 1.f); // Yellow
            break;

        case ERROR :
            textColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
            break;

        case INFO :
            textColor = ImVec4(0.46f, 0.96f, 0.46f, 1.f); // Green
            break;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::TextUnformatted(entry.message.c_str());
        ImGui::PopStyleColor();
    }
}

void Console::clearConsole()
{
    for(auto &entry : m_LogEntries)
    {
        m_LogEntries.clear();
    }
}

void Console::InputText()
{
    // INPUT TEXT
    ImGui::PushItemWidth(-ImGui::GetStyle().ItemSpacing.x * 7);
    if (ImGui::InputText("Input", m_InputBuffer, sizeof(m_InputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {

        LogMessage("> " + std::string(m_InputBuffer), LogType::COMMAND);

        if(!commandManager.ExecuteCommand(m_InputBuffer))
        {
            LogMessage("\tUnknown command: '" + std::string(m_InputBuffer) + "'", LogType::WARRNING);
        }
        LogMessage("\n", LogType::LOG);


        m_InputBuffer[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1);
    }
}
