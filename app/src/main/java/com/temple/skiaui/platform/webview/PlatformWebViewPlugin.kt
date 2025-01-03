package com.temple.skiaui.platform.webview

import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformTextureLayerBasePlugin

class PlatformWebViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, webViewPtr: Long) :
    PlatformTextureLayerBasePlugin(engine, width, height, webViewPtr),
    IWebViewCallback {

    private var webView: PlatformWebView? = null

    override fun onProgressChanged(progress: Int) {
        engine.postToSkiaUI {
            if (viewPtr != 0L) {
                engine.webViewProgressChange(viewPtr, progress)
            }
        }
    }

    override fun initPlatformView(): View {
        val webView = PlatformWebView(engine.view.context)
        webView.setCanvasProvider(this)
        webView.callback = this
        this.webView = webView
        return webView
    }

    override fun destroyPlatformView() {
        webView?.destroy()
        webView = null
    }

    fun loadUrl(url: String) {
        mainHandler.post {
            webView?.loadUrl(url)
        }
    }

}