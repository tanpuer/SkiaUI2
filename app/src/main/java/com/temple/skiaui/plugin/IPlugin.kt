package com.temple.skiaui.plugin

interface IPlugin {

    fun getName(): String

    fun invoke(methodName: String, methodParam: String): String

}