package com.temple.skiaui.inspect

import android.os.Handler
import android.os.HandlerThread
import com.temple.skiaui.HYSkiaEngine
import java.net.InetSocketAddress

class InspectPlugin(
    private val engine: HYSkiaEngine,
    private val nativePtr: Long,
    private val port: Int
) {

    private var index = "surface-texture:${INDEX++}"

    private val pluginThread = HandlerThread(index).apply {
        start()
    }

    private val pluginHandler = Handler(pluginThread.looper)

    private val sendThread = HandlerThread("send:${INDEX++}").apply {
        start()
    }

    private val sendHandler = Handler(sendThread.looper)

    @Volatile
    private var server: InspectServer? = null

    fun run() {
        pluginHandler.post {
            val address = InetSocketAddress(port)
            server = InspectServer(engine, nativePtr, address)
            server?.run()
        }
    }

    fun sendMessage(message: String) {
        sendHandler.post {
            server?.sendMessage(message)
        }
    }

    fun startListening() {
        pluginHandler.post {
            server?.start()
        }
    }

    fun waitFrontendMessage(): String? {
        return server?.waitFrontendMessage()
    }

    fun printLogInChrome(log: String) {
        sendHandler.post {
            server?.sendMessage("{\"method\":\"Runtime.consoleAPICalled\",\"params\":{\"type\":\"log\",\"args\":[{\"type\":\"string\",\"value\":\"${log}\"}]}}")
        }
    }

    fun release() {
        server?.stop()
        server = null
        pluginThread.quitSafely()
        sendThread.quitSafely()
    }

    companion object {
        private var INDEX = 0
        private const val TAG = "InspectServer"
    }

}