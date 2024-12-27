package com.temple.skiaui.platform.edittext

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatEditText
import com.temple.skiaui.platform.ICanvasProvider

class PlatformEditText @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : AppCompatEditText(context, attrs) {

    init {
        hint = "this is Android EditText"
    }

    private var render: ICanvasProvider? = null

    fun setCanvasProvider(render: ICanvasProvider) {
        this.render = render
    }

    override fun onDraw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas() ?: return
        glCanvas.drawColor(Color.WHITE)
        super.onDraw(glCanvas)
        render?.unLockCanvas(glCanvas)
    }

}