package com.temple.skiaui.plugin

import android.os.Handler
import android.os.Looper
import android.widget.Toast
import com.temple.skiaui.HYSkiaUIApp

class ToastPlugin : IPlugin {

    override fun getName(): String = "toast"

    private val mainHandler = Handler(Looper.getMainLooper())

    private var toast: Toast? = null

    override fun invoke(methodName: String, methodParam: String): String {
        mainHandler.post {
            if (methodName == "show") {
                toast?.cancel()
                toast = Toast.makeText(HYSkiaUIApp.getInstance(), methodParam, Toast.LENGTH_SHORT)
                toast?.show()
            }
        }
        return ""
    }

}