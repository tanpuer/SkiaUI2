package com.temple.skiaui.compose.foundation

import com.temple.skiaui.compose.ui.Align
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.Justify
import com.temple.skiaui.compose.ui.Position

fun Modifier.setSize(width: Int, height: Int): Modifier {
    styles["size"] = intArrayOf(width, height)
    return this
}

fun Modifier.setBackgroundColor(color: String): Modifier {
    styles["backgroundColor"] = color
    return this
}

fun Modifier.setWidth(width: Int): Modifier {
    styles["width"] = width
    return this
}

fun Modifier.setHeight(height: Int): Modifier {
    styles["height"] = height
    return this
}

fun Modifier.setTextSize(size: Int): Modifier {
    styles["textSize"] = size
    return this
}

fun Modifier.setJustifyContent(value: Justify): Modifier {
    styles["justifyContent"] = value
    return this
}

fun Modifier.setAlignItems(value: Align): Modifier {
    styles["alignItems"] = value
    return this
}

fun Modifier.setFlexDirection(value: FlexDirection): Modifier {
    styles["flexDirection"] = value
    return this
}

fun Modifier.setPosition(value: Position): Modifier {
    styles["position"] = value
    return this
}

fun Modifier.setMargins(margins: IntArray): Modifier {
    styles["margins"] = margins
    return this
}

fun Modifier.setCorner(radius: Int): Modifier {
    styles["corner"] = radius
    return this
}