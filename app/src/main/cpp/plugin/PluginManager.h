//
// Created by ByteDance on 2023/1/6.
//

#ifndef SKIAUI_PLUGINMANAGER_H
#define SKIAUI_PLUGINMANAGER_H

#include "IPlugin.h"

class PluginManager {

private:

    PluginManager();

public:

    PluginManager(const PluginManager &manager) = delete;

    void operator=(PluginManager &manager) = delete;

    ~PluginManager();

    void registerPlugin(IPlugin *plugin);

    void clearPlugins();

    static PluginManager &getInstance() {
        static PluginManager manager;
        return manager;
    }

};


#endif //SKIAUI_PLUGINMANAGER_H
