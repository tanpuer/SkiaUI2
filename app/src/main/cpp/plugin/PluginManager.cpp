#include "PluginManager.h"
#include "assert.h"
#include "native_log.h"


PluginManager::PluginManager() {

}

PluginManager::~PluginManager() {

}

std::string PluginManager::invokeMethod(const std::string &pluginName,
                                        const std::string &methodName,
                                        const std::string &methodParam) {
    if (globalJavaPlugins != nullptr && this->env != nullptr) {
        auto result = (jstring) env->CallObjectMethod(globalJavaPlugins, javaInvokeMethod,
                                                      env->NewStringUTF(pluginName.c_str()),
                                                      env->NewStringUTF(methodName.c_str()),
                                                      env->NewStringUTF(methodParam.c_str()));
        return env->GetStringUTFChars(result, nullptr);
    }
    static std::string tmp = "";
    return tmp;
}

void PluginManager::initJavaPluginManager(jobject javaPlugins, JNIEnv *env) {
    globalJavaPlugins = env->NewGlobalRef(javaPlugins);
    javaPluginsClass = env->FindClass("com/temple/skiaui/plugin/PluginManager");
    assert(javaPluginsClass);
    javaInvokeMethod = env->GetMethodID(javaPluginsClass, "invokeMethod",
                                        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    assert(javaInvokeMethod);
    this->env = env;
}

void PluginManager::releaseJavaPluginManager(JNIEnv *env) {
    env->DeleteGlobalRef(globalJavaPlugins);
    globalJavaPlugins = nullptr;
    javaPluginsClass = nullptr;
    javaInvokeMethod = nullptr;
    this->env = nullptr;
}

