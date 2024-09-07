#include "CommandManager.h"

void CommandManager::RegisterCommand(const std::string &name, const std::string &description, std::function<void(const std::vector<std::string> &)> execute)
{
    commands.emplace_back(name, description, execute);
}

int CommandManager::ExecuteCommand(const std::string input)
{
    for (auto command : commands)
    {
        if(command.Name == input)
        {
            command.Execute({});
            return 1;
        }
    }
    if(input == " ")
        return 1;
    else
        return 0;
}

