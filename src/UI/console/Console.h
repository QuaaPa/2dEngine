#ifndef CONSOLE_H
#define CONSOLE_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../../pch.h"

#include "../CommandManager.h"

enum LogType
{
    COMMAND,
    LOG,
    WARRNING,
    ERROR,
    INFO
};

struct LogEntry {
    std::string message;
    LogType type;
};

class Console
{
private:
    static char m_InputBuffer[255];

    static std::vector<LogEntry> m_LogEntries;

public:
    static void Initialize();

    static void ShowConsole();

    static void LogMessage(const std::string& message, LogType type);

private:
    static void InputText();

    static void Render();

    static void clearConsole();
};

#endif // CONSOLE_H