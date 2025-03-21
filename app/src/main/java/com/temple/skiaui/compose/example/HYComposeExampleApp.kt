package com.temple.skiaui.compose.example

import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.core.HYComposeApplication
import com.temple.skiaui.compose.core.HYComposeSDK

class HYComposeExampleApp(engine: HYSkiaEngine, context: Long) :
    HYComposeApplication(engine, context) {

    init {
        engine.registerJetpackCompose()
        HYComposeSDK.initSDK(engine)
    }

    override fun onCreate(width: Int, height: Int) {
        super.onCreate(width, height)
        HYComposeExamplePage(engine, context).start(width, height)
    }

    override fun onDestroy() {

    }

}
