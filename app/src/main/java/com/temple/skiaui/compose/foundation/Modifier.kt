package com.temple.skiaui.compose.foundation

import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.ui.HYComposeView

class Modifier {

    val styles = mutableMapOf<String, Any>()

    val attributes = mutableMapOf<String, Any>()

    val events = mutableMapOf<String, (view: HYComposeView, params: String?) -> Unit>()

    companion object {

        fun size(width: Int, height: Int): Modifier {
            return Modifier().size(width, height)
        }

        fun width(width: Int): Modifier {
            return Modifier().width(width)
        }

        fun height(height: Int): Modifier {
            return Modifier().height(height)
        }

        fun textSize(size: Dp): Modifier {
            return Modifier().textSize(size)
        }

        fun margins(margins: IntArray): Modifier {
            return Modifier().margins(margins)
        }

    }

}
