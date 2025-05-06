package com.temple.skiaui.compose.example

import android.util.Log
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.R
import com.temple.skiaui.compose.example.theme.JetpackComposeTheme
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.alignItems
import com.temple.skiaui.compose.foundation.margins
import com.temple.skiaui.compose.foundation.size
import com.temple.skiaui.compose.runtime.EditText
import com.temple.skiaui.compose.runtime.HYComposeBasePage
import com.temple.skiaui.compose.runtime.Scroll
import com.temple.skiaui.compose.runtime.Web
import com.temple.skiaui.compose.ui.Align

class HYComposeNativeViewsPage(engine: HYSkiaEngine) : HYComposeBasePage(engine) {

    @Composable
    override fun RunComposable(width: Dp, height: Dp) {
        JetpackComposeTheme {
            DisposableEffect(engine.getContext()) {
                onDispose {
                    Log.d(TAG, "onDispose")
                }
            }
            Scroll(
                modifier = Modifier().size(width, height)
                    .alignItems(Align.Center),
                backgroundColor = MaterialTheme.colorScheme.background
            ) {
                EditText(
                    modifier = Modifier.size(width, 70.dp)
                        .margins(arrayOf(0.dp, 20.dp, 0.dp, 0.dp)),
                    hint = stringResource(R.string.edit_hint_string),
                    focus = false
                )
                Web(
                    Modifier.size(width, height - 90.dp),
                    url = "https://m.bilibili.com/"
                )
            }
        }
    }

    companion object {
        private const val TAG = "HYComposeAnimationPage"
    }
}
