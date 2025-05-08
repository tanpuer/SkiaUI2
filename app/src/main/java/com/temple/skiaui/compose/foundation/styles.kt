package com.temple.skiaui.compose.foundation

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.DpSize
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.FlexWrap
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.Position

fun Modifier.size(width: Dp, height: Dp): Modifier {
    this.width = width
    this.height = height
    return this
}

fun Modifier.backgroundColor(color: Color): Modifier {
    this.backgroundColor = color
    return this
}

fun Modifier.width(width: Dp): Modifier {
    this.width = width
    return this
}

fun Modifier.height(height: Dp): Modifier {
    this.height = height
    return this
}

fun Modifier.justifyContent(value: Justify): Modifier {
    this.justifyContent = value
    return this
}

fun Modifier.alignItems(value: Align): Modifier {
    this.alignItems = value
    return this
}

fun Modifier.alignSelf(value: Align): Modifier {
    this.alignSelf = value
    return this
}

fun Modifier.flexDirection(value: FlexDirection): Modifier {
    this.flexDirection = value
    return this
}

fun Modifier.position(value: Position): Modifier {
    this.position = value
    return this
}

fun Modifier.margins(margins: Array<Dp>): Modifier {
    this.margins = margins
    return this
}

fun Modifier.paddings(paddings: Array<Dp>): Modifier {
    this.paddings = paddings
    return this
}

fun Modifier.corner(radius: Dp): Modifier {
    this.corner = radius
    return this
}

fun Modifier.minSize(size: DpSize): Modifier {
    this.minSize = size
    return this
}

fun Modifier.wrap(wrap: FlexWrap): Modifier {
    this.wrap = wrap
    return this
}