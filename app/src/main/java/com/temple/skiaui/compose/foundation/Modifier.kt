package com.temple.skiaui.compose.foundation

import androidx.annotation.FloatRange
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.isUnspecified
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.DpSize
import androidx.compose.ui.unit.isUnspecified
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.FlexWrap
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

    var flexDirection: FlexDirection = FlexDirection.Unspecified

    var justifyContent: Justify = Justify.Unspecified

    var alignItems: Align = Align.Unspecified

    var alignSelf: Align = Align.Unspecified

    var wrap: FlexWrap = FlexWrap.Unspecified

    @FloatRange(0.0, 100.0)
    var widthPercent: Float = 0.0f

    @FloatRange(0.0, 100.0)
    var heightPercent: Float = 0.0f

    var flex = -1

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
        if (margins != null && (previous.margins == null ||
                    previous.margins?.getOrNull(0) != margins?.getOrNull(0) ||
                    previous.margins?.getOrNull(1) != margins?.getOrNull(1) ||
                    previous.margins?.getOrNull(2) != margins?.getOrNull(2) ||
                    previous.margins?.getOrNull(3) != margins?.getOrNull(3))
        ) {
            margins?.let {
                styles[StyleKey.margins] = it
            }
        }
        if (paddings != null && (previous.paddings == null ||
                    previous.paddings?.getOrNull(0) != paddings?.getOrNull(0) ||
                    previous.paddings?.getOrNull(1) != paddings?.getOrNull(1) ||
                    previous.paddings?.getOrNull(2) != paddings?.getOrNull(2) ||
                    previous.paddings?.getOrNull(3) != paddings?.getOrNull(3))
        ) {
            paddings?.let {
                styles[StyleKey.paddings] = it
            }
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
        if (alignSelf != Align.Unspecified && alignSelf != previous.alignSelf) {
            styles[StyleKey.alignSelf] = alignSelf
        }
        if (wrap != FlexWrap.Unspecified && wrap != previous.wrap) {
            styles[StyleKey.wrap] = wrap
        }
        if (widthPercent > 0 && widthPercent != previous.widthPercent) {
            styles[StyleKey.widthPercent] = widthPercent
        }
        if (heightPercent > 0 && heightPercent != previous.heightPercent) {
            styles[StyleKey.heightPercent] = heightPercent
        }
        if (flex > 0 && flex != previous.flex) {
            styles[StyleKey.flex] = flex
        }
    }

    val styles = mutableMapOf<String, Any>()

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

        fun margins(margins: Array<Dp>): Modifier {
            return Modifier().margins(margins)
        }

        fun backgroundColor(color: Color): Modifier {
            return Modifier().backgroundColor(color)
        }

        fun justifyContent(value: Justify): Modifier {
            return Modifier().justifyContent(value)
        }

        fun alignItems(value: Align): Modifier {
            return Modifier().alignItems(value)
        }

    }

}
