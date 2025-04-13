package com.temple.skiaui.platform.edittext

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.PorterDuff
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

    private var glDirty = true

    fun setCanvasProvider(render: ICanvasProvider) {
        this.render = render
    }

    /**
     * EditText sometimes draw dark, TODO!
     */
    override fun onDraw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas(canvas)
        if (glCanvas == null) {
            invalidate()
            return
        }
        glDirty = false
        glCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
        glCanvas.save()
        glCanvas.translate(-scrollX.toFloat(), -scrollY.toFloat())
        super.onDraw(glCanvas)
        glCanvas.restore()
        render?.unLockCanvas(glCanvas)
    }

    override fun isDirty(): Boolean {
        return super.isDirty() || glDirty
    }

}