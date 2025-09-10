package com.temple.skiaui.platform.edittext

import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformTextureLayerBasePlugin

class PlatformEditTextPlugin(engine: HYSkiaEngine, editTextPtr: Long) :
    PlatformTextureLayerBasePlugin(engine, editTextPtr) {

    private var editText: PlatformEditText? = null

    override fun initPlatformView(): View {
        val editText = PlatformEditText(engine.view.context)
        editText.setCanvasProvider(this)
        this.editText = editText
        return editText
    }

    override fun destroyPlatformView() {
        editText?.clearFocus()
    }

    private fun clearFocus() {
        mainHandler.post {
            editText?.clearFocus()
        }
    }

    private fun requestFocus() {
        mainHandler.post {
            editText?.requestFocus()
        }
    }

    private fun setHint(hint: String) {
        mainHandler.post {
            editText?.hint = hint
        }
    }

    private fun setTextColor(color: Int) {
        mainHandler.post {
            editText?.setTextColor(color)
        }
    }

    private fun setHintColor(color: Int) {
        mainHandler.post {
            editText?.setHintTextColor(color)
        }
    }

    private fun setInputType(type: Int) {
        mainHandler.post {
            editText?.inputType = type
        }
    }

    override fun onSizeChange(left: Int, top: Int, width: Int, height: Int) {
        super.onSizeChange(left, top, width, height)
        mainHandler.post {
            editText?.clearFocus()
        }
    }

}