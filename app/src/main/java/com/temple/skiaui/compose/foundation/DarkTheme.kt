package com.temple.skiaui.compose.foundation

import android.content.res.Configuration
import androidx.compose.runtime.Composable
import androidx.compose.runtime.ReadOnlyComposable
import androidx.compose.ui.platform.LocalConfiguration

@Composable
@ReadOnlyComposable
internal fun isSystemInDarkTheme(): Boolean {
    val uiMode = LocalConfiguration.current.uiMode
    return (uiMode and Configuration.UI_MODE_NIGHT_MASK) == Configuration.UI_MODE_NIGHT_YES
}