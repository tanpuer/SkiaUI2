package com.temple.skiaui.platform.edittext

import android.graphics.Color
import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformTextureLayerBasePlugin
import androidx.core.graphics.toColorInt

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
        editText?.clearFocus()
    }

    fun clearFocus() {
        mainHandler.post {
            editText?.clearFocus()
        }
    }

    fun requestFocus() {
        mainHandler.post {
            editText?.requestFocus()
        }
    }

    fun setHint(hint: String) {
        mainHandler.post {
            editText?.hint = hint
        }
    }

    fun setTextColor(color: Int) {
        mainHandler.post {
            editText?.setTextColor(color)
        }
    }

    fun setHintColor(color: Int) {
        mainHandler.post {
            editText?.setHintTextColor(color)
        }
    }

}