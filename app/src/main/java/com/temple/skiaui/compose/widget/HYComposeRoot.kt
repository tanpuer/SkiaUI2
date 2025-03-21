package com.temple.skiaui.compose.widget

import com.temple.skiaui.compose.foundation.Modifier

class HYComposeRoot(modifier: Modifier) : HYComposeView(modifier) {

    override fun getViewType(): String {
        return "Root"
    }

}