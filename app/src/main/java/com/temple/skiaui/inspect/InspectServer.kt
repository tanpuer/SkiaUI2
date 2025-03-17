package com.temple.skiaui.inspect

import android.util.Log
import com.temple.skiaui.HYSkiaEngine
import org.java_websocket.WebSocket
import org.java_websocket.handshake.ClientHandshake
import org.java_websocket.server.WebSocketServer
import java.lang.Exception
import java.net.InetSocketAddress
import java.util.concurrent.ConcurrentLinkedDeque

class InspectServer(
    private val engine: HYSkiaEngine,
    private val nativePtr: Long,
    address: InetSocketAddress
) : WebSocketServer(address) {

    @Volatile
    private var paused = false

    private val list = ConcurrentLinkedDeque<String>()

    override fun onOpen(conn: WebSocket?, handshake: ClientHandshake?) {
        Log.d(TAG, "onOpen")
    }

    override fun onClose(conn: WebSocket?, code: Int, reason: String?, remote: Boolean) {
        Log.d(TAG, "onClose")
    }

    fun sendMessage(message: String?) {
        Log.d(TAG, "broadcast $message")
        if (message?.contains("Debugger.paused") == true) {
            paused = true
        }
        broadcast(message)
    }

    override fun onMessage(conn: WebSocket?, message: String?) {
        Log.d(TAG, "onMessage $message")
        if (message?.contains("Debugger.resume") == true) {
            list.push(message)
            paused = false
        } else {
            if (paused) {
                list.push(message ?: "")
            } else {
                engine.sendInspectMsg(message ?: "", nativePtr)
            }
        }
    }

    override fun onError(conn: WebSocket?, ex: Exception?) {
        Log.d(TAG, "onError: ${ex?.message}")
    }

    override fun onStart() {
        Log.d(TAG, "onStart")
    }

    fun waitFrontendMessage(): String? {
        if (list.size == 0) {
            return null
        }
        return list.poll() ?: ""
    }

    companion object {
        private const val TAG = "InspectServer"
    }

}