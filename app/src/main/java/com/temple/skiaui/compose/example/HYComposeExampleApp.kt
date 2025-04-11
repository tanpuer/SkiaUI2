package com.temple.skiaui.compose.example

import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.runtime.HYComposeApplication
import com.temple.skiaui.compose.runtime.HYComposeSDK

class HYComposeExampleApp(engine: HYSkiaEngine, context: Long) :
    HYComposeApplication(engine, context) {

    init {
        engine.registerJetpackCompose()
        HYComposeSDK.initSDK(engine)
    }

    override fun onCreate(width: Int, height: Int) {
        super.onCreate(width, height)
        HYComposeExamplePage(engine, context).apply {
            start(width, height)
            HYComposeSDK.pushPage(this)
        }
    }

    override fun onDestroy() {

    }

}
