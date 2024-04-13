#pragma once

#include "string"

class IPlugin {

public:

    enum class PluginType {
        Toast,
    };

public:

    IPlugin(std::string name);

    virtual ~IPlugin();

    virtual std::string invoke(const std::string &methodName, const std::string &methodParam) = 0;

    const char *getName() { return name.c_str(); }

    IPlugin() = delete;

    IPlugin(IPlugin &plugin) = delete;

    IPlugin &operator=(IPlugin &plugin) = delete;

    IPlugin(IPlugin &&plugin) = delete;

private:

    std::string name;

};