package com.temple.skiaui.platform.edittext

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.PorterDuff
import android.graphics.Rect
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatEditText
import com.temple.skiaui.platform.ICanvasProvider

class PlatformEditText @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : AppCompatEditText(context, attrs) {

    private var showAndroidUI = false

    init {
        hint = "this is Android EditText"
        background = null
        setSingleLine()
    }

    private var render: ICanvasProvider? = null

    private var glDirty = true

    fun setCanvasProvider(render: ICanvasProvider) {
        this.render = render
    }

    override fun onFocusChanged(focused: Boolean, direction: Int, previouslyFocusedRect: Rect?) {
        super.onFocusChanged(focused, direction, previouslyFocusedRect)
        showAndroidUI = focused
        invalidate()
    }

    override fun draw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas(canvas)
        if (glCanvas == null) {
            invalidate()
            return
        }
        glDirty = false
        glCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
        glCanvas.save()
        glCanvas.translate(-scrollX.toFloat(), -scrollY.toFloat())
        super.draw(glCanvas)
        glCanvas.restore()
        render?.unLockCanvas(glCanvas)
        if (showAndroidUI) {
            super.draw(canvas)
        }
    }

    override fun isDirty(): Boolean {
        return super.isDirty() || glDirty
    }

}