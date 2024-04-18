package com.temple.skiaui.plugin

import android.widget.Toast
import com.temple.skiaui.HYSkiaUIApp

class ToastPlugin : IPlugin {

    override fun getName(): String = "toast"

    override fun invoke(methodName: String, methodParam: String): String {
        if (methodName == "show") {
            Toast.makeText(HYSkiaUIApp.getInstance(), methodParam, Toast.LENGTH_SHORT).show()
        }
        return ""
    }

}