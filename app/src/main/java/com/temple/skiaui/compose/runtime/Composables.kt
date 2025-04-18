package com.temple.skiaui.compose.runtime

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ComposeNode
import androidx.compose.ui.graphics.Color
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.ui.CameraCallback
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.HYComposeButton
import com.temple.skiaui.compose.ui.HYComposeCamera
import com.temple.skiaui.compose.ui.HYComposeEditText
import com.temple.skiaui.compose.ui.HYComposeExoVideo
import com.temple.skiaui.compose.ui.HYComposeFilament
import com.temple.skiaui.compose.ui.HYComposeFlexboxLayout
import com.temple.skiaui.compose.ui.HYComposeIcon
import com.temple.skiaui.compose.ui.HYComposeImage
import com.temple.skiaui.compose.ui.HYComposeLoadingView
import com.temple.skiaui.compose.ui.HYComposeLottie
import com.temple.skiaui.compose.ui.HYComposeSVG
import com.temple.skiaui.compose.ui.HYComposeScrollView
import com.temple.skiaui.compose.ui.HYComposeShader
import com.temple.skiaui.compose.ui.HYComposeSwitch
import com.temple.skiaui.compose.ui.HYComposeText
import com.temple.skiaui.compose.ui.HYComposeVideo
import com.temple.skiaui.compose.ui.HYComposeView
import com.temple.skiaui.compose.ui.HYComposeWeb

@Composable
fun View(modifier: Modifier, backgroundColor: Color, rotateZ: Float = 0.0f) {
    ComposeNode<HYComposeView, HYComposeApplier>(
        factory = { HYComposeView(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                this.setBackgroundColor(backgroundColor)
            }
            set(rotateZ) {
                this.setRotateZ(rotateZ)
            }
        }
    )
}

@Composable
fun Column(modifier: Modifier, backgroundColor: Color, content: @Composable () -> Unit) {
    ComposeNode<HYComposeFlexboxLayout, HYComposeApplier>(
        factory = { HYComposeFlexboxLayout(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                this.setBackgroundColor(backgroundColor)
            }
        },
        content = content
    )
}

@Composable
fun Row(modifier: Modifier, content: @Composable () -> Unit) {
    ComposeNode<HYComposeFlexboxLayout, HYComposeApplier>(
        factory = { HYComposeFlexboxLayout(modifier).apply { setFlexDirection(FlexDirection.Row) } },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
        },
        content = content
    )
}

@Composable
fun LazyColumn(modifier: Modifier, backgroundColor: Color, content: @Composable () -> Unit) {
    ComposeNode<HYComposeScrollView, HYComposeApplier>(
        factory = { HYComposeScrollView(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                this.setBackgroundColor(backgroundColor)
            }
        },
        content = content
    )
}

@Composable
fun Video(modifier: Modifier, source: String) {
    ComposeNode<HYComposeVideo, HYComposeApplier>(
        factory = { HYComposeVideo(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setSource(source)
            }
        }
    )
}

@Composable
fun ExoVideo(modifier: Modifier, source: String) {
    ComposeNode<HYComposeExoVideo, HYComposeApplier>(
        factory = { HYComposeExoVideo(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setSource(source)
            }
        }
    )
}

@Composable
fun Loading(modifier: Modifier, color: Color = Color.Blue) {
    ComposeNode<HYComposeLoadingView, HYComposeApplier>(
        factory = { HYComposeLoadingView(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(color) {
                setColor(color)
            }
        }
    )
}

@Composable
fun SVG(modifier: Modifier, source: String, rotateZ: Float = 0.0f) {
    ComposeNode<HYComposeSVG, HYComposeApplier>(
        factory = { HYComposeSVG(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setSource(source)
            }
            set(rotateZ) {
                setRotateZ(rotateZ)
            }
        }
    )
}

@Composable
fun Lottie(modifier: Modifier, source: String, play: Boolean = true) {
    ComposeNode<HYComposeLottie, HYComposeApplier>(
        factory = { HYComposeLottie(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setSource(source)
            }
            set(play) {
                if (play) start() else pause()
            }
        }
    )
}

@Composable
fun Shader(modifier: Modifier, shaderSource: ShaderSource) {
    ComposeNode<HYComposeShader, HYComposeApplier>(
        factory = { HYComposeShader(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(shaderSource) {
                setShaderSource(shaderSource)
            }
        }
    )
}

@Composable
fun Text(
    modifier: Modifier,
    content: String,
    color: Color,
    maxLine: Int = 0,
    ellipsis: String = "…"
) {
    ComposeNode<HYComposeText, HYComposeApplier>(
        factory = { HYComposeText(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(content) {
                setText(content)
            }
            set(color) {
                setColor(color)
            }
            set(maxLine) {
                setMaxLine(maxLine)
            }
            set(ellipsis) {
                setEllipse(ellipsis)
            }
        }
    )
}

@Composable
fun Button(
    modifier: Modifier,
    content: String,
    color: Color = Color.White
) {
    ComposeNode<HYComposeText, HYComposeApplier>(
        factory = { HYComposeButton(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(content) {
                setText(content)
            }
            set(color) {
                setColor(color)
            }
        }
    )
}

@Composable
fun Image(modifier: Modifier, source: String) {
    ComposeNode<HYComposeImage, HYComposeApplier>(
        factory = { HYComposeImage(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setImageSource(source)
            }
        }
    )
}

@Composable
fun Icon(icon: Int = 0, color: Color = Color.Blue, size: Int = 100, modifier: Modifier = Modifier()) {
    ComposeNode<HYComposeIcon, HYComposeApplier>(
        factory = { HYComposeIcon(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(icon) {
                setIcon(icon)
            }
            set(color) {
                setIconColor(color)
            }
            set(size) {
                setIconSize(size)
            }
        }
    )
}

@Composable
fun Switch(modifier: Modifier, enable: Boolean = true, color: String = "#ff00ff") {
    ComposeNode<HYComposeSwitch, HYComposeApplier>(
        factory = { HYComposeSwitch(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(enable) {
                setEnable(enable)
            }
            set(color) {
                setColor(color)
            }
        }
    )
}

@Composable
fun Camera(modifier: Modifier, callback: CameraCallback) {
    ComposeNode<HYComposeCamera, HYComposeApplier>(
        factory = { HYComposeCamera(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(callback) {
                capture(callback)
            }
        }
    )
}

@Composable
fun Filament(modifier: Modifier) {
    ComposeNode<HYComposeFilament, HYComposeApplier>(
        factory = { HYComposeFilament(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
        }
    )
}

@Composable
fun Web(modifier: Modifier, url: String) {
    ComposeNode<HYComposeWeb, HYComposeApplier>(
        factory = { HYComposeWeb(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(url) {
                loadUrl(url)
            }
        }
    )
}

@Composable
fun EditText(modifier: Modifier, hint: String, focus: Boolean = false) {
    ComposeNode<HYComposeEditText, HYComposeApplier>(
        factory = { HYComposeEditText(modifier, hint) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(hint) {
                setHint(hint)
            }
            set(focus) {
                setFocus(focus)
            }
        }
    )
}

