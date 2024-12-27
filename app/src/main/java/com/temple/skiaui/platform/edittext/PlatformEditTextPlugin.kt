package com.temple.skiaui.platform.edittext

import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformBasePlugin

class PlatformEditTextPlugin(engine: HYSkiaEngine, width: Int, height: Int, webViewPtr: Long) :
    PlatformBasePlugin(engine, width, height, webViewPtr) {

    private var editText: PlatformEditText? = null

    override fun initPlatformView(): View {
        val editText = PlatformEditText(engine.view.context)
        editText.setCanvasProvider(this)
        this.editText = editText
        return editText
    }

    override fun destroyPlatformView() {

    }

}