package com.temple.skiaui.platform.webview

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.PorterDuff
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.webkit.WebChromeClient
import android.webkit.WebResourceRequest
import android.webkit.WebView
import android.webkit.WebViewClient
import com.temple.skiaui.platform.ICanvasProvider

@SuppressLint("SetJavaScriptEnabled")
class PlatformWebView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : WebView(context, attrs) {

    var callback: IWebViewCallback? = null

    init {
        settings.javaScriptEnabled = true
        webViewClient = HYWebViewClient()
        webChromeClient = HYWebChromeClient()
    }

    private var render: ICanvasProvider? = null

    fun setCanvasProvider(render: ICanvasProvider) {
        this.render = render
    }

    override fun onDraw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas(canvas)
        if (glCanvas == null) {
            invalidate()
            return
        }
        glCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
        glCanvas.save()
        glCanvas.translate(-scrollX.toFloat(), -scrollY.toFloat())
        super.onDraw(glCanvas)
        glCanvas.restore()
        render?.unLockCanvas(glCanvas)
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        Log.d(TAG, "${event?.action}  ${event?.x}  ${event?.y}")
        return super.onTouchEvent(event)
    }

    inner class HYWebViewClient : WebViewClient() {
        override fun shouldOverrideUrlLoading(
            view: WebView?,
            request: WebResourceRequest?
        ): Boolean {
            val url = request?.url?.toString()
            if (url?.startsWith("http://") == true || url?.startsWith("https://") == true) {
                return false
            }
            return true
        }
    }

    inner class HYWebChromeClient : WebChromeClient() {
        override fun onProgressChanged(view: WebView?, newProgress: Int) {
            super.onProgressChanged(view, newProgress)
            callback?.onProgressChanged(newProgress)
        }
    }

    companion object {
        private const val TAG = "PlatformWebView"
    }

}