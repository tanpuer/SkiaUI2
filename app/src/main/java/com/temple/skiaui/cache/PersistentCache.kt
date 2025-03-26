package com.temple.skiaui.cache

import com.temple.skiaui.HYSkiaUIApp
import java.io.File
import java.nio.ByteBuffer
import java.security.MessageDigest
import java.util.concurrent.ConcurrentHashMap
import java.util.concurrent.Executors

object PersistentCache {

    private val ctx = HYSkiaUIApp.getInstance()
    private const val CACHE_DIR_NAME = "skia_cache"
    private val cacheDir by lazy {
        File(ctx.filesDir, CACHE_DIR_NAME).apply { mkdirs() }
    }
    private val cacheMap = ConcurrentHashMap<String, ByteBuffer>()
    private val executors = Executors.newFixedThreadPool(1)

    init {
        executors.submit {
            cacheDir.list()?.forEach {
                val file = File(cacheDir, it)
                val byteBuffer =
                    ByteBuffer.allocateDirect(file.length().toInt()).put(file.readBytes())
                cacheMap[it] = byteBuffer
            }
        }
    }

    fun store(key: ByteBuffer, value: ByteBuffer) {
        val keyHash = hashKey(key)
        val byteArray = if (value.hasArray()) {
            value.array()
        } else {
            ByteArray(value.remaining()).apply {
                value.duplicate().get(this)
            }
        }
        executors.submit {
            File(cacheDir, keyHash).writeBytes(byteArray)
        }
    }

    fun load(key: ByteBuffer): ByteBuffer? {
        val keyHash = hashKey(key)
        if (cacheMap.containsKey(keyHash)) {
            return cacheMap[keyHash]
        }
        val file = File(cacheDir, keyHash)
        if (!file.exists()) {
            return null
        }
        return ByteBuffer.allocateDirect(file.length().toInt()).put(file.readBytes())
    }

    private fun hashKey(key: ByteBuffer): String {
        val md = MessageDigest.getInstance("SHA-1")
        md.update(key)
        return md.digest().joinToString("")
    }

}