package com.temple.skiaui.compose.foundation

class Modifier {

    val styles = mutableMapOf<String, Any>()

    val attributes = mutableMapOf<String, Any>()

    val events = mutableMapOf<String, (params: Any) -> Unit>()

}