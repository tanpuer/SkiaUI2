package com.temple.skiaui.plugin

object PluginManager {

    fun registerPlugin(plugin: IPlugin) {
        plugins[plugin.getName()] = plugin
    }

    fun clearPlugins() {
        plugins.clear()
    }

    fun invokeMethod(pluginName: String, methodName: String, methodParam: String): String {
        return plugins[pluginName]?.invoke(methodName, methodParam) ?: ""
    }

    private val plugins = mutableMapOf<String, IPlugin>()

}