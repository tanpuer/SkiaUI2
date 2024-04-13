#pragma once

#include <jni.h>
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

    void clearPlugins();

    std::string invokeMethod(const std::string &pluginName,
                             const std::string &methodName,
                             const std::string &methodParam);

    static PluginManager *getInstance() {
        static PluginManager manager;
        return &manager;
    }

    void initJavaPluginManager(JNIEnv *env);

private:

    std::unordered_map<std::string, IPlugin *> plugins;

};
