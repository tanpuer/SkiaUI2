package com.temple.skiaui.plugin

class PluginManager {

    private val plugins = mutableMapOf<String, IPlugin>()

    init {
        registerPlugin(ToastPlugin())
    }

    fun registerPlugin(plugin: IPlugin) {
        plugins[plugin.getName()] = plugin
    }

    fun clearPlugins() {
        plugins.clear()
    }

    fun invokeMethod(pluginName: String, methodName: String, methodParam: String): String {
        return plugins[pluginName]?.invoke(methodName, methodParam) ?: ""
    }

}