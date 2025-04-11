package com.temple.skiaui.compose.foundation

import com.temple.skiaui.compose.ui.HYComposeView

fun Modifier.onClick(callback: (view: HYComposeView) -> Unit): Modifier {
    events["click"] = { view, _ ->
        callback(view)
    }
    return this
}