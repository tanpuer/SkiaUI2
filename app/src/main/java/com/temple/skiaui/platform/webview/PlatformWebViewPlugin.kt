package com.temple.skiaui.platform.webview

import android.os.Looper
import android.view.View
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformTextureLayerBasePlugin

class PlatformWebViewPlugin(engine: HYSkiaEngine, webViewPtr: Long) :
    PlatformTextureLayerBasePlugin(engine, webViewPtr), IWebViewCallback {

    private var webView: PlatformWebView? = null

    private var url: String? = null

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
        url?.let {
            webView.loadUrl(it)
        }
        this.webView = webView
        return webView
    }

    override fun destroyPlatformView() {
        webView?.destroy()
        webView = null
    }

    private fun loadUrl(url: String) {
        mainHandler.post {
            this.url = url
            webView?.loadUrl(url)
        }
    }

    private fun canGoBack(): Boolean {
        if (Looper.myLooper() == Looper.getMainLooper()) {
            return webView?.canGoBack() ?: false
        }
        return false
    }

}