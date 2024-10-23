package com.temple.skiaui.compose.foundation

fun Modifier.setSource(source: String): Modifier {
    attributes["source"] = source
    return this
}