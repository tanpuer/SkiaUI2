package com.temple.skiaui.compose.ui

import com.temple.skiaui.compose.foundation.Modifier

open class HYComposeScrollView(modifier: Modifier) : HYComposeFlexboxLayout(modifier) {

    override fun getViewType(): String = "Scroll"

}