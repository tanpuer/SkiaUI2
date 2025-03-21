package com.temple.skiaui.compose.core

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ComposeNode
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.widget.CameraCallback
import com.temple.skiaui.compose.widget.HYComposeCamera
import com.temple.skiaui.compose.widget.HYComposeExoVideo
import com.temple.skiaui.compose.widget.HYComposeFilament
import com.temple.skiaui.compose.widget.HYComposeFlexboxLayout
import com.temple.skiaui.compose.widget.HYComposeIcon
import com.temple.skiaui.compose.widget.HYComposeImage
import com.temple.skiaui.compose.widget.HYComposeLoadingView
import com.temple.skiaui.compose.widget.HYComposeLottie
import com.temple.skiaui.compose.widget.HYComposePage
import com.temple.skiaui.compose.widget.HYComposeSVG
import com.temple.skiaui.compose.widget.HYComposeScrollView
import com.temple.skiaui.compose.widget.HYComposeShader
import com.temple.skiaui.compose.widget.HYComposeSwitch
import com.temple.skiaui.compose.widget.HYComposeText
import com.temple.skiaui.compose.widget.HYComposeVideo
import com.temple.skiaui.compose.widget.HYComposeView
import com.temple.skiaui.compose.widget.HYComposeWeb

@Composable
fun View(modifier: Modifier, backgroundColor: String) {
    ComposeNode<HYComposeView, HYComposeApplier>(
        factory = { HYComposeView(modifier) },
        update = {
            set(backgroundColor) {
                this.setBackgroundColor(backgroundColor)
            }
        }
    )
}

@Composable
fun Column(modifier: Modifier, backgroundColor: String, content: @Composable () -> Unit) {
    ComposeNode<HYComposeFlexboxLayout, HYComposeApplier>(
        factory = { HYComposeFlexboxLayout(modifier) },
        update = {
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
        factory = { HYComposeFlexboxLayout(modifier).apply { setFlexDirection("row") } },
        update = {},
        content = content
    )
}

@Composable
fun LazyColumn(modifier: Modifier, backgroundColor: String, content: @Composable () -> Unit) {
    ComposeNode<HYComposeScrollView, HYComposeApplier>(
        factory = { HYComposeScrollView(modifier) },
        update = {
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
            set(source) {
                setSource(source)
            }
        }
    )
}

@Composable
fun Page(modifier: Modifier, content: @Composable () -> Unit) {
    ComposeNode<HYComposePage, HYComposeApplier>(
        factory = {
            val page = HYComposePage(modifier)
            page.push((modifier.styles["size"] as IntArray)[0])
            page
        },
        update = {},
        content = content
    )
}

@Composable
fun Loading(modifier: Modifier, color: String = "") {
    ComposeNode<HYComposeLoadingView, HYComposeApplier>(
        factory = { HYComposeLoadingView(modifier) },
        update = {
            set(color) {
                setColor(color)
            }
        }
    )
}

@Composable
fun SVG(modifier: Modifier, source: String) {
    ComposeNode<HYComposeSVG, HYComposeApplier>(
        factory = { HYComposeSVG(modifier) },
        update = {
            set(source) {
                setSource(source)
            }
        }
    )
}

@Composable
fun Lottie(modifier: Modifier, source: String, play: Boolean = true) {
    ComposeNode<HYComposeLottie, HYComposeApplier>(
        factory = { HYComposeLottie(modifier) },
        update = {
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
    color: String,
    maxLine: Int = 0,
    ellipsis: String = "…"
) {
    ComposeNode<HYComposeText, HYComposeApplier>(
        factory = { HYComposeText(modifier) },
        update = {
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
fun Image(modifier: Modifier, source: String) {
    ComposeNode<HYComposeImage, HYComposeApplier>(
        factory = { HYComposeImage(modifier) },
        update = {
            set(source) {
                setImageSource(source)
            }
        }
    )
}

@Composable
fun Icon(modifier: Modifier, icon: Int, color: String = "#0000ff", size: Int = 100) {
    ComposeNode<HYComposeIcon, HYComposeApplier>(
        factory = { HYComposeIcon(modifier) },
        update = {
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

        }
    )
}

@Composable
fun Web(modifier: Modifier, url: String) {
    ComposeNode<HYComposeWeb, HYComposeApplier>(
        factory = { HYComposeWeb(modifier) },
        update = {
            set(url) {
                loadUrl(url)
            }
        }
    )
}
