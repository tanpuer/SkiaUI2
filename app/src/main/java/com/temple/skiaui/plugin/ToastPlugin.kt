package com.temple.skiaui.plugin

import android.os.Handler
import android.os.Looper
import android.view.Gravity
import android.widget.Toast
import com.temple.skiaui.HYSkiaUIApp

class ToastPlugin : IPlugin {

    override fun getName(): String = "toast"

    private val mainHandler = Handler(Looper.getMainLooper())

    override fun invoke(methodName: String, methodParam: String): String {
        mainHandler.post {
            if (methodName == "show") {
                Toast.makeText(HYSkiaUIApp.getInstance(), methodParam, Toast.LENGTH_SHORT).apply {
                    setGravity(Gravity.BOTTOM, 0, 0)
                    show()
                }
            }
        }
        return ""
    }

}