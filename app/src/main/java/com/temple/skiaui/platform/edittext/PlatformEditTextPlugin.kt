package com.temple.skiaui.platform.edittext

import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformTextureLayerBasePlugin

class PlatformEditTextPlugin(engine: HYSkiaEngine, width: Int, height: Int, editTextPtr: Long) :
    PlatformTextureLayerBasePlugin(engine, width, height, editTextPtr) {

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