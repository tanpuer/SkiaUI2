#include "PluginManager.h"
#include "assert.h"
#include "native_log.h"

PluginManager::PluginManager() {

}

PluginManager::~PluginManager() {

}

void PluginManager::registerPlugin(IPlugin *plugin) {
    assert(plugins.find(plugin->getName()) == plugins.end());
    plugins[plugin->getName()] = plugin;
}

void PluginManager::clearPlugins() {
    for (const auto &item: plugins) {
        delete item.second;
    }
    plugins.clear();
}

std::string PluginManager::invokeMethod(const std::string &pluginName,
                                        const std::string &methodName,
                                        const std::string &methodParam) {
    if (plugins.find(pluginName) == plugins.end()) {
        ALOGE("invalid pluginName: %s", pluginName.c_str())
        static std::string empty = "";
        return empty;
    }
    return plugins[pluginName]->invoke(methodName, methodParam);
}

void PluginManager::initJavaPluginManager(JNIEnv *env) {
    const char *javaPluginManagerPath = "com/temple/skiaui/plugin/PluginManager";
    auto javaPluginManager = env->FindClass(javaPluginManagerPath);
    assert(javaPluginManager != nullptr);
}
