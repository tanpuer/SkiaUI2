package com.temple.skiaui.compose.material

import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
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
    val paddingTop = if (content.isEmpty()) 8.dp else 5.dp
    Text(
        modifier = Modifier.textSize(10.dp)
            .corner(Int.MAX_VALUE.dp)
            .backgroundColor(containerColor)
            .paddings(arrayOf(9.dp, paddingTop, 9.dp, paddingTop)),
        content, contentColor
    )
}