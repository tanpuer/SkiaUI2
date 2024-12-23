package com.temple.skiaui.platform

import android.content.Context
import android.graphics.Canvas
import android.util.AttributeSet
import android.webkit.WebChromeClient
import android.webkit.WebResourceRequest
import android.webkit.WebResourceResponse
import android.webkit.WebView
import android.webkit.WebViewClient

class PlatformWebView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : WebView(context, attrs) {

    init {
        settings.javaScriptEnabled = true
        webViewClient = WebViewClient()
        webChromeClient = WebChromeClient()
    }

    private var render: PlatformWebViewPlugin? = null

    fun setPlatformRenderer(render: PlatformWebViewPlugin) {
        this.render = render
    }

    override fun onDraw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas() ?: return
        super.onDraw(glCanvas)
        render?.unLockCanvas(glCanvas)
    }

    inner class HYWebChromeClient: WebViewClient() {
        override fun shouldOverrideUrlLoading(
            view: WebView?,
            request: WebResourceRequest?
        ): Boolean {
            view?.loadUrl(request?.url.toString())
            return true
        }
    }

}
