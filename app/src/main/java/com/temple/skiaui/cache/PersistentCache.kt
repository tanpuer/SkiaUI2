package com.temple.skiaui.cache

import android.util.Log
import com.temple.skiaui.HYSkiaUIApp
import java.io.File
import java.nio.ByteBuffer
import java.security.MessageDigest
import java.util.concurrent.ConcurrentHashMap
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

/**
 * shader program binaries are stored and loaded in GL thread
 */
object PersistentCache {

    private const val TAG = "PersistentCache"
    private val ctx = HYSkiaUIApp.getInstance()
    private const val CACHE_DIR_NAME = "skia_cache"
    private val cacheDir by lazy {
        File(ctx.filesDir, CACHE_DIR_NAME).apply { mkdirs() }
    }
    private val cacheMap = ConcurrentHashMap<String, ByteBuffer>()
    private var executors: ExecutorService? = null
    private var preloaded = false

    fun preload(executors: ExecutorService) {
        PersistentCache.executors = executors
        if (preloaded) {
            return
        }
        executors.submit {
            cacheDir.list()?.forEach {
                val file = File(cacheDir, it)
                val byteBuffer =
                    ByteBuffer.allocateDirect(file.length().toInt()).put(file.readBytes())
                cacheMap[it] = byteBuffer
            }
        }
        preloaded = true
    }

    fun store(key: ByteBuffer, value: ByteBuffer) {
        Log.d(TAG, "store")
        val keyHash = hashKey(key)
        val byteArray = if (value.hasArray()) {
            value.array()
        } else {
            ByteArray(value.remaining()).apply {
                value.duplicate().get(this)
            }
        }
        executors?.submit {
            File(cacheDir, keyHash).writeBytes(byteArray)
        }
    }

    fun load(key: ByteBuffer): ByteBuffer? {
        val keyHash = hashKey(key)
        if (cacheMap.containsKey(keyHash)) {
            Log.d(TAG, "cache hit")
            return cacheMap[keyHash]
        }
        Log.d(TAG, "cache missed, try to read file")
        val file = File(cacheDir, keyHash)
        if (!file.exists()) {
            return null
        }
        val result = ByteBuffer.allocateDirect(file.length().toInt()).put(file.readBytes())
        cacheMap[keyHash] = result
        return result
    }

    private fun hashKey(key: ByteBuffer): String {
        val md = MessageDigest.getInstance("SHA-1")
        md.update(key)
        return md.digest().joinToString("")
    }

}