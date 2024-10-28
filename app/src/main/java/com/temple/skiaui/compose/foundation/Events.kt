package com.temple.skiaui.compose.foundation

import com.temple.skiaui.compose.widget.HYComposeView

fun Modifier.onClick(callback: (view: HYComposeView) -> Unit): Modifier {
    events["click"] = callback
    return this
}