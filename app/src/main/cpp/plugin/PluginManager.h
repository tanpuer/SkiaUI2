#pragma once

#include <jni.h>
#include "unordered_map"
#include "string"

namespace HYSkiaUI {

class PluginManager {

public:

    PluginManager();

    ~PluginManager();

    std::string invokeMethod(const std::string &pluginName,
                             const std::string &methodName,
                             const std::string &methodParam);

    void initJavaPluginManager(jobject javaPlugins, JNIEnv *env);

    void releaseJavaPluginManager(JNIEnv *env);

private:

    jobject globalJavaPlugins = nullptr;
    jclass javaPluginsClass = nullptr;
    jmethodID javaInvokeMethod = nullptr;

    JNIEnv *env = nullptr;

};

}
