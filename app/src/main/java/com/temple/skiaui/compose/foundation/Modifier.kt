package com.temple.skiaui.compose.foundation

import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.ui.HYComposeView

class Modifier {

    val styles = mutableMapOf<String, Any>()

    val attributes = mutableMapOf<String, Any>()

    val events = mutableMapOf<String, (view: HYComposeView, params: String?) -> Unit>()

    companion object {

        fun size(width: Dp, height: Dp): Modifier {
            return Modifier().size(width, height)
        }

        fun width(width: Dp): Modifier {
            return Modifier().width(width)
        }

        fun height(height: Dp): Modifier {
            return Modifier().height(height)
        }

        fun textSize(size: Dp): Modifier {
            return Modifier().textSize(size)
        }

        fun margins(margins: Array<Dp>): Modifier {
            return Modifier().margins(margins)
        }

    }

}
