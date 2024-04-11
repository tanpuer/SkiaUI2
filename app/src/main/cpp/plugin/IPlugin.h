#pragma once

#include "string"

class IPlugin {

public:

    IPlugin(std::string name);

    virtual ~IPlugin();

    virtual void invoke(const std::string &methodName, const std::string &methodParam) = 0;

    IPlugin() = delete;

    IPlugin(IPlugin &plugin) = delete;

    IPlugin &operator=(IPlugin &plugin) = delete;

    IPlugin(IPlugin &&plugin) = delete;

private:

    std::string name;

};