package com.temple.skiaui.compose.foundation

import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.Position

fun Modifier.size(width: Int, height: Int): Modifier {
    styles["size"] = intArrayOf(width, height)
    return this
}

fun Modifier.backgroundColor(color: Color): Modifier {
    styles["backgroundColor"] = color
    return this
}

fun Modifier.width(width: Int): Modifier {
    styles["width"] = width
    return this
}

fun Modifier.height(height: Int): Modifier {
    styles["height"] = height
    return this
}

fun Modifier.textSize(size: Dp): Modifier {
    styles["textSize"] = size
    return this
}

fun Modifier.justifyContent(value: Justify): Modifier {
    styles["justifyContent"] = value
    return this
}

fun Modifier.alignItems(value: Align): Modifier {
    styles["alignItems"] = value
    return this
}

fun Modifier.flexDirection(value: FlexDirection): Modifier {
    styles["flexDirection"] = value
    return this
}

fun Modifier.position(value: Position): Modifier {
    styles["position"] = value
    return this
}

fun Modifier.margins(margins: IntArray): Modifier {
    styles["margins"] = margins
    return this
}

fun Modifier.paddings(paddings: IntArray): Modifier {
    styles["paddings"] = paddings
    return this
}

fun Modifier.corner(radius: Int): Modifier {
    styles["corner"] = radius
    return this
}

fun Modifier.minSize(minWidth: Int, minHeight: Int): Modifier {
    styles["minSize"] = intArrayOf(minWidth, minHeight)
    return this
}