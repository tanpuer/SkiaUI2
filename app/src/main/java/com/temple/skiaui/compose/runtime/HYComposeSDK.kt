package com.temple.skiaui.compose.runtime

import android.util.Log
import com.temple.skiaui.HYSkiaEngine
import java.util.Stack

object HYComposeSDK {

    private const val TAG = "HYComposeSDK"

    private lateinit var pageStack: Stack<HYComposeBasePage>

    fun initSDK(engine: HYSkiaEngine) {
        pageStack = Stack<HYComposeBasePage>()
    }

    fun DeInitSDK() {
        pageStack.clear()
    }

    fun pushPage(page: HYComposeBasePage) {
        Log.d(TAG, "pushPage")
        pageStack.push(page)
    }

    fun popPage(engine: HYSkiaEngine) {
        engine.onBackPressed()
    }

    fun onPagePoped() {
        if (pageStack.empty()) {
            Log.e(TAG, "popPage failed")
            return
        }
        val page = pageStack.pop()
        page.dispose()
        if (pageStack.empty()) {
            DeInitSDK()
        }
    }

    fun onUIModeChange() {
        pageStack.forEach {
            it.onUIModeChange()
        }
    }

}
