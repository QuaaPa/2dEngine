#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <functional>
#include "../pch.h"

struct Command
{
    std::string Name;
    std::string Description;
    std::function<void(const std::vector<std::string>&)> Execute;

    Command(const std::string& name,
            const std::string& description,
            std::function<void(const std::vector<std::string>&)> execute)
        : Name(name), Description(description), Execute(execute)
    {
        // Nothing
    }
};

class CommandManager
{
public:
    void RegisterCommand(const std::string& name,
                         const std::string& description,
                         std::function<void(const std::vector<std::string>&)> execute);

    int ExecuteCommand(const std::string input);

    std::vector<Command> commands;

private:

};

#endif // COMMAND_MANAGER_H