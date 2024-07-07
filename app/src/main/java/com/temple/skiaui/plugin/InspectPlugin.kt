package com.temple.skiaui.plugin

class InspectPlugin : IPlugin {
    override fun getName(): String = "inspect"

    override fun invoke(methodName: String, methodParam: String): String {
        if (methodName == "run") {

        } else if (methodName == "sendMessage") {

        }
        return ""
    }
}