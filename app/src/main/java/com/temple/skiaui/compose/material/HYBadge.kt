package com.temple.skiaui.compose.material

import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.backgroundColor
import com.temple.skiaui.compose.foundation.corner
import com.temple.skiaui.compose.foundation.paddings
import com.temple.skiaui.compose.runtime.Text

@Composable
fun Badge(
    content: String,
    containerColor: Color = Color.Magenta,
    contentColor: Color = Color.White
) {
    val paddingTop = if (content.isEmpty()) 20 else 10
    Text(
        modifier = Modifier.textSize(30)
            .corner(Int.MAX_VALUE)
            .backgroundColor(containerColor)
            .paddings(intArrayOf(20, paddingTop, 20, paddingTop)),
        content, contentColor
    )
}