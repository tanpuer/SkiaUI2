package com.temple.skiaui.compose.foundation

import com.temple.skiaui.compose.ui.HYComposeView

class Modifier {

    val styles = mutableMapOf<String, Any>()

    val attributes = mutableMapOf<String, Any>()

    val events = mutableMapOf<String, (view: HYComposeView, params: String?) -> Unit>()

}