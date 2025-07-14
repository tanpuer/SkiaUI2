package com.temple.skiaui.compose.runtime

import android.graphics.Bitmap
import android.view.Surface
import androidx.annotation.DrawableRes
import androidx.compose.runtime.Composable
import androidx.compose.runtime.ComposeNode
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.ui.AutoReleasable
import com.temple.skiaui.compose.ui.CameraCallback
import com.temple.skiaui.compose.ui.Canvas
import com.temple.skiaui.compose.ui.ContentScale
import com.temple.skiaui.compose.ui.FlexDirection
import com.temple.skiaui.compose.ui.HYComposeAndroidImage
import com.temple.skiaui.compose.ui.HYComposeButton
import com.temple.skiaui.compose.ui.HYComposeCamera
import com.temple.skiaui.compose.ui.HYComposeCanvas
import com.temple.skiaui.compose.ui.HYComposeEditText
import com.temple.skiaui.compose.ui.HYComposeExoVideo
import com.temple.skiaui.compose.ui.HYComposeFilament
import com.temple.skiaui.compose.ui.HYComposeFlexboxLayout
import com.temple.skiaui.compose.ui.HYComposeIcon
import com.temple.skiaui.compose.ui.HYComposeImage
import com.temple.skiaui.compose.ui.HYComposeLoadingView
import com.temple.skiaui.compose.ui.HYComposeLottie
import com.temple.skiaui.compose.ui.HYComposePaint
import com.temple.skiaui.compose.ui.HYComposePath
import com.temple.skiaui.compose.ui.HYComposeProgressBar
import com.temple.skiaui.compose.ui.HYComposeRecyclerView
import com.temple.skiaui.compose.ui.HYComposeSVG
import com.temple.skiaui.compose.ui.HYComposeScrollView
import com.temple.skiaui.compose.ui.HYComposeShader
import com.temple.skiaui.compose.ui.HYComposeSwitch
import com.temple.skiaui.compose.ui.HYComposeText
import com.temple.skiaui.compose.ui.HYComposeTextureView
import com.temple.skiaui.compose.ui.HYComposeView
import com.temple.skiaui.compose.ui.HYComposeWeb
import com.temple.skiaui.compose.ui.TextAlign
import com.temple.skiaui.compose.ui.TextGradient
import com.temple.skiaui.compose.ui.util.decodeDrawableResource
import com.temple.skiaui.platform.video.IVideoPlayer

@Composable
fun View(
    modifier: Modifier,
    backgroundColor: Color,
    rotateZ: Float = 0.0f,
    onClick: (() -> Unit)? = null
) {
    ComposeNode<HYComposeView, HYComposeApplier>(
        factory = { HYComposeView(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                setBackgroundColor(backgroundColor)
            }
            set(rotateZ) {
                setRotateZ(rotateZ)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Column(
    modifier: Modifier,
    content: @Composable () -> Unit
) {
    ComposeNode<HYComposeFlexboxLayout, HYComposeApplier>(
        factory = { HYComposeFlexboxLayout(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
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
fun Scroll(
    ref: ((scroll: HYComposeScrollView) -> Unit)? = null, //TODO, should support ref?
    modifier: Modifier,
    backgroundColor: Color = Color.Transparent,
    offset: Int = -1,
    content: @Composable () -> Unit
) {
    ComposeNode<HYComposeScrollView, HYComposeApplier>(
        factory = {
            HYComposeScrollView(modifier).apply {
                ref?.invoke(this)
            }
        },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                setBackgroundColor(backgroundColor)
            }
            set(offset) {
                scrollTo(offset)
            }
        },
        content = content
    )
}

@Composable
fun ExoVideo(
    modifier: Modifier,
    customPlayer: IVideoPlayer? = null,
    source: String,
    onClick: (() -> Unit)? = null,
    shaderPath: String? = null,
    shaderCode: String? = null,
    backgroundPlayback: Boolean? = null,
    repeat: Boolean? = null
) {
    ComposeNode<HYComposeExoVideo, HYComposeApplier>(
        factory = { HYComposeExoVideo(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(customPlayer) {
                setCustomPlayer(customPlayer)
            }
            set(source) {
                setSource(source)
            }
            set(onClick) {
                setOnClick(onClick)
            }
            set(shaderPath) {
                setShaderPath(shaderPath)
            }
            set(shaderCode) {
                setShaderCode(shaderCode)
            }
            set(backgroundPlayback) {
                setBackgroundPlayback(backgroundPlayback)
            }
            set(repeat) {
                setRepeatMode(repeat)
            }
        }
    )
}

@Composable
fun Loading(modifier: Modifier, color: Color = Color.Blue, onClick: (() -> Unit)? = null) {
    ComposeNode<HYComposeLoadingView, HYComposeApplier>(
        factory = { HYComposeLoadingView(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(color) {
                setColor(color)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun SVG(
    modifier: Modifier,
    source: String,
    rotateZ: Float = 0.0f,
    onClick: (() -> Unit)? = null
) {
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
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Lottie(
    modifier: Modifier,
    source: String,
    play: Boolean = true,
    onClick: (() -> Unit)? = null
) {
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
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Shader(modifier: Modifier, shaderSource: ShaderSource, onClick: (() -> Unit)? = null) {
    ComposeNode<HYComposeShader, HYComposeApplier>(
        factory = { HYComposeShader(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(shaderSource) {
                setShaderSource(shaderSource)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Text(
    modifier: Modifier,
    content: String,
    textSize: Dp = 20.dp,
    color: Color = Color.Black,
    textAlign: TextAlign = TextAlign.Unspecified,
    maxLine: Int = 0,
    ellipsis: String = "…",
    fontFamily: String? = null,
    textGradient: TextGradient? = null,
    onClick: (() -> Unit)? = null
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
            set(textSize) {
                setTextSize(textSize)
            }
            set(color) {
                setColor(color)
            }
            set(textAlign) {
                setTextAlign(textAlign)
            }
            set(maxLine) {
                setMaxLine(maxLine)
            }
            set(ellipsis) {
                setEllipse(ellipsis)
            }
            set(fontFamily) {
                setFontFamily(fontFamily)
            }
            set(textGradient) {
                setTextGradient(textGradient)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Button(
    modifier: Modifier,
    content: String,
    textSize: Dp = 20.dp,
    color: Color = Color.White,
    onClick: (() -> Unit)? = null
) {
    ComposeNode<HYComposeText, HYComposeApplier>(
        factory = { HYComposeButton(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(textSize) {
                setTextSize(textSize)
            }
            set(content) {
                setText(content)
            }
            set(color) {
                setColor(color)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Image(modifier: Modifier, source: String, onClick: (() -> Unit)? = null) {
    ComposeNode<HYComposeImage, HYComposeApplier>(
        factory = { HYComposeImage(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setImageSource(source)
            }
            set(onClick) {
                setOnClick(onClick)
            }
        }
    )
}

@Composable
fun Icon(
    icon: Int = 0,
    color: Color = Color.Blue,
    size: Int = 100,
    modifier: Modifier = Modifier(),
    onClick: (() -> Unit)? = null
) {
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
            set(onClick) {
                setOnClick(onClick)
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
fun Camera(
    modifier: Modifier,
    callback: CameraCallback,
    shaderPath: String? = null,
    shaderCode: String? = null
) {
    ComposeNode<HYComposeCamera, HYComposeApplier>(
        factory = { HYComposeCamera(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(callback) {
                capture(callback)
            }
            set(shaderPath) {
                setShaderPath(shaderPath)
            }
            set(shaderCode) {
                setShaderCode(shaderCode)
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
fun EditText(
    modifier: Modifier,
    hint: String,
    focus: Boolean = false,
    textColor: Color = Color.Unspecified,
    hintColor: Color = Color.Unspecified,
    inputType: Int = -1
) {
    ComposeNode<HYComposeEditText, HYComposeApplier>(
        factory = { HYComposeEditText(modifier) },
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
            set(textColor) {
                setTextColor(textColor)
            }
            set(hintColor) {
                setHintColor(hintColor)
            }
            set(inputType) {
                setInputType(inputType)
            }
        }
    )
}

@Composable
fun ProgressBar(
    modifier: Modifier,
    barColor: Color = Color.Red,
    backgroundColor: Color = Color.Green,
    progress: Int = 0,
    onChange: ((progress: Int, finished: Boolean) -> Unit)? = null
) {
    ComposeNode<HYComposeProgressBar, HYComposeApplier>(
        factory = { HYComposeProgressBar(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(barColor) {
                setBarColor(barColor)
            }
            set(backgroundColor) {
                setBackgroundColor(backgroundColor)
            }
            set(onChange) {
                setOnChangeListener(onChange)
            }
            set(progress) {
                setProgress(progress)
            }
        }
    )
}

/**
 * For ComposeRecyclerView, createView/bindView/viewType are called from Native, it is impossible to change them to @Composable functions.
 * So use RecyclerView is not comfortable compared with other widgets.
 */
@Composable
fun RecyclerView(
    modifier: Modifier,
    backgroundColor: Color = Color.Unspecified,
    createView: (index: Int) -> HYComposeView,
    bindView: (index: Int, view: HYComposeView) -> Unit,
    viewType: (index: Int) -> Int,
    dataSize: Int
) {
    ComposeNode<HYComposeRecyclerView, HYComposeApplier>(
        factory = { HYComposeRecyclerView(modifier, createView, bindView, viewType, dataSize) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(backgroundColor) {
                setBackgroundColor(backgroundColor)
            }
            set(dataSize) {
                setDataSize(dataSize)
            }
        },
    )
}

@Composable
fun AndroidImage(
    modifier: Modifier,
    source: String? = null,
    @DrawableRes resId: Int = 0,
    contentScale: ContentScale = ContentScale.Unspecified,
    blur: Float = 0.0f,
    rotateZ: Float = 0.0f,
    onClick: (() -> Unit)? = null
) {
    ComposeNode<HYComposeAndroidImage, HYComposeApplier>(
        factory = { HYComposeAndroidImage(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(source) {
                setResource(source)
            }
            set(resId) {
                setResId(resId)
            }
            set(onClick) {
                setOnClick(onClick)
            }
            set(contentScale) {
                setContentScale(contentScale)
            }
            set(blur) {
                setBlur(blur)
            }
            set(rotateZ) {
                setRotateZ(rotateZ)
            }
        }
    )
}

@Composable
fun Canvas(modifier: Modifier, onDraw: (canvas: Canvas) -> Unit) {
    ComposeNode<HYComposeCanvas, HYComposeApplier>(
        factory = { HYComposeCanvas(modifier) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(onDraw) {
                setDrawCallback(onDraw)
            }
        }
    )
}

@Composable
fun TextureView(
    modifier: Modifier,
    engine: HYSkiaEngine,
    surfaceCreate: ((surface: Surface) -> Unit),
    surfaceChange: ((surface: Surface, width: Int, height: Int) -> Unit),
    surfaceDestroy: ((surface: Surface) -> Unit),
    show: () -> Unit,
    hide: () -> Unit
) {
    ComposeNode<HYComposeTextureView, HYComposeApplier>(
        factory = { HYComposeTextureView(modifier, engine) },
        update = {
            set(modifier) {
                updateModifier(modifier)
            }
            set(surfaceCreate) {
                onSurfaceTextureCreated(surfaceCreate)
            }
            set(surfaceChange) {
                onSurfaceTextureChanged(surfaceChange)
            }
            set(surfaceDestroy) {
                onSurfaceTextureDestroyed(surfaceDestroy)
            }
            set(show) {
                onShow(show)
            }
            set(hide) {
                onHide(hide)
            }
        }
    )
}

@Composable
fun <T : AutoReleasable> rememberAutoReleasable(
    factory: () -> T
): T {
    val autoReleasable = remember { factory() }
    DisposableEffect(Unit) {
        onDispose {
            autoReleasable.release()
        }
    }
    return autoReleasable
}

@Composable
fun rememberAutoReleasePaint(): HYComposePaint {
    return rememberAutoReleasable { HYComposePaint() }
}

@Composable
fun rememberAutoReleasePath(): HYComposePath {
    return rememberAutoReleasable { HYComposePath() }
}

@Composable
fun rememberAutoReleaseBitmap(@DrawableRes resId: Int, reqWidth: Int = Int.MAX_VALUE): Bitmap {
    val resources = LocalContext.current.resources
    val bitmap = remember {
        decodeDrawableResource(resources, resId, reqWidth)
    }
    DisposableEffect(Unit) {
        onDispose {
            bitmap.recycle()
        }
    }
    return bitmap
}
