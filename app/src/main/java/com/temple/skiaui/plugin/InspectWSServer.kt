package com.temple.skiaui.plugin

import org.java_websocket.WebSocket
import org.java_websocket.handshake.ClientHandshake
import org.java_websocket.server.WebSocketServer
import java.lang.Exception
import java.net.InetSocketAddress

class InspectWSServer(port: Int): WebSocketServer(InetSocketAddress(port)) {

    private var conn: WebSocket? = null

    override fun onOpen(conn: WebSocket?, handshake: ClientHandshake?) {
        this.conn = conn
    }

    override fun onClose(conn: WebSocket?, code: Int, reason: String?, remote: Boolean) {
        this.conn = null
    }

    override fun onMessage(conn: WebSocket?, message: String?) {

    }

    override fun onError(conn: WebSocket?, ex: Exception?) {
        this.conn = null
    }

    override fun onStart() {

    }

    fun send(message: String) {
        this.conn?.send(message)
    }

    companion object {
        private const val TAG = "Inspect-WebServer"
    }

}