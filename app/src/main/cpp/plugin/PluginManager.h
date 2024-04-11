#pragma once

#include "IPlugin.h"
#include "unordered_map"

class PluginManager {

private:

    PluginManager();

public:

    PluginManager(const PluginManager &manager) = delete;

    void operator=(PluginManager &manager) = delete;

    ~PluginManager();

    void registerPlugin(IPlugin *plugin);

    void unRegisterPlugin(IPlugin *plugin);

    void clearPlugins();

    static PluginManager *getInstance() {
        static PluginManager manager;
        return &manager;
    }

private:

    std::unordered_map<std::string, IPlugin *> plugins;

};
