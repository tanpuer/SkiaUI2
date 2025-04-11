package com.temple.skiaui.compose.foundation

import com.temple.skiaui.compose.ui.HYComposeView

class Modifier {

    val styles = mutableMapOf<String, Any>()

    val attributes = mutableMapOf<String, Any>()

    val events = mutableMapOf<String, (view: HYComposeView, params: String?) -> Unit>()

    companion object {

        fun setSize(width: Int, height: Int): Modifier {
            return Modifier().setSize(width, height)
        }

        fun setWidth(width: Int): Modifier {
            return Modifier().setWidth(width)
        }

        fun setHeight(height: Int): Modifier {
            return Modifier().setHeight(height)
        }

        fun setTextSize(size: Int): Modifier {
            return Modifier().setTextSize(size)
        }

        fun setMargins(margins: IntArray): Modifier {
            return Modifier().setMargins(margins)
        }

    }

}
