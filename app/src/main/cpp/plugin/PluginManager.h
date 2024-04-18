#pragma once

#include <jni.h>
#include "unordered_map"
#include "string"

class PluginManager {

private:

    PluginManager();

public:

    PluginManager(const PluginManager &manager) = delete;

    void operator=(PluginManager &manager) = delete;

    ~PluginManager();

    std::string invokeMethod(const std::string &pluginName,
                             const std::string &methodName,
                             const std::string &methodParam);

    static PluginManager *getInstance() {
        static PluginManager manager;
        return &manager;
    }

    void initJavaPluginManager(jobject javaPlugins, JNIEnv *env);

    void releaseJavaPluginManager(JNIEnv *env);

private:

    jobject globalJavaPlugins = nullptr;
    jclass javaPluginsClass = nullptr;
    jmethodID javaInvokeMethod = nullptr;

    JNIEnv *env = nullptr;

};
