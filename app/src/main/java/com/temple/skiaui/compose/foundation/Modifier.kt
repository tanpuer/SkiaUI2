package com.temple.skiaui.compose.foundation

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.isUnspecified
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.DpSize
import androidx.compose.ui.unit.isUnspecified
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.HYComposeView
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.Position

class Modifier {

    var width: Dp = Dp.Unspecified

    var height: Dp = Dp.Unspecified

    var margins: Array<Dp>? = null

    var paddings: Array<Dp>? = null

    var backgroundColor: Color = Color.Unspecified

    var position: Position = Position.Unspecified

    var corner: Dp = Dp.Unspecified

    var minSize: DpSize = DpSize.Unspecified

    var textSize: Dp = Dp.Unspecified

    var flexDirection: FlexDirection = FlexDirection.Unspecified

    var justifyContent: Justify = Justify.Unspecified

    var alignItems: Align = Align.Unspecified

    fun diffStyles(previous: Modifier) {
        if (!width.isUnspecified && width != previous.width) {
            styles[StyleKey.width] = width
        }
        if (!height.isUnspecified && height != previous.height) {
            styles[StyleKey.height] = height
        }
        if (!backgroundColor.isUnspecified && backgroundColor != previous.backgroundColor) {
            styles[StyleKey.backgroundColor] = backgroundColor
        }
        if (position != Position.Unspecified && position != previous.position) {
            styles[StyleKey.position] = position
        }
        if (!corner.isUnspecified && corner != previous.corner) {
            styles[StyleKey.corner] = corner
        }
        if (!minSize.isUnspecified && minSize != previous.minSize) {
            styles[StyleKey.minSize] = minSize
        }
        if (!textSize.isUnspecified && textSize != previous.textSize) {
            styles[StyleKey.textSize] = textSize
        }
        if (flexDirection != FlexDirection.Unspecified && flexDirection != previous.flexDirection) {
            styles[StyleKey.flexDirection] = flexDirection
        }
        if (justifyContent != Justify.Unspecified && justifyContent != previous.justifyContent) {
            styles[StyleKey.justifyContent] = justifyContent
        }
        if (alignItems != Align.Unspecified && alignItems != previous.alignItems) {
            styles[StyleKey.alignItems] = alignItems
        }
        margins?.let {
            styles[StyleKey.margins] = it
        }
        paddings?.let {
            styles[StyleKey.paddings] = it
        }
    }

    val styles = mutableMapOf<String, Any>()

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
