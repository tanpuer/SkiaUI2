package com.temple.skiaui.compose.core

import android.util.Log
import android.view.Choreographer
import androidx.compose.runtime.BroadcastFrameClock
import androidx.compose.runtime.Composable
import androidx.compose.runtime.ComposeNode
import androidx.compose.runtime.ControlledComposition
import androidx.compose.runtime.Recomposer
import androidx.compose.runtime.snapshots.Snapshot
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.compose.foundation.Modifier
import com.temple.skiaui.compose.foundation.ShaderSource
import com.temple.skiaui.compose.foundation.setBackgroundColor
import com.temple.skiaui.compose.foundation.setSize
import com.temple.skiaui.compose.widget.CameraCallback
import com.temple.skiaui.compose.widget.HYComposeCamera
import com.temple.skiaui.compose.widget.HYComposeFlexboxLayout
import com.temple.skiaui.compose.widget.HYComposeIcon
import com.temple.skiaui.compose.widget.HYComposeImage
import com.temple.skiaui.compose.widget.HYComposeLoadingView
import com.temple.skiaui.compose.widget.HYComposeLottie
import com.temple.skiaui.compose.widget.HYComposePage
import com.temple.skiaui.compose.widget.HYComposeScrollView
import com.temple.skiaui.compose.widget.HYComposeShader
import com.temple.skiaui.compose.widget.HYComposeSwitch
import com.temple.skiaui.compose.widget.HYComposeText
import com.temple.skiaui.compose.widget.HYComposeVideo
import com.temple.skiaui.compose.widget.HYComposeView
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.CoroutineStart
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

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
fun Page(modifier: Modifier) {
    ComposeNode<HYComposePage, HYComposeApplier>(
        factory = { HYComposePage(modifier) },
        update = {}
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

fun runCompose(
    content: @Composable () -> Unit,
    width: Int,
    height: Int,
    engine: HYSkiaEngine,
    context: Long
): Long {
    val frameClock = BroadcastFrameClock()
    val reComposer = Recomposer(frameClock)
    val rootNode = HYComposePage(
        Modifier(context)
            .setSize(width, height)
            .setBackgroundColor("#ffffffff")
    )
    val composition = ControlledComposition(
        applier = HYComposeApplier(rootNode),
        parent = reComposer
    )
    composition.setContent(content)
    val coroutineContext: CoroutineContext by lazy {
        val dispatcher = HYComposeUIDispatcher(
            Choreographer.getInstance(),
            engine
        )
        dispatcher + dispatcher.frameClock
    }
    val scope = CoroutineScope(coroutineContext)
    scope.launch {
        var snapshotJob: Job? = null
        val snapshotHandle = Snapshot.registerGlobalWriteObserver {
            if (snapshotJob == null) {
                snapshotJob = scope.launch {
                    snapshotJob = null
                    Snapshot.sendApplyNotifications()
                }
            }
        }
        val recomposeJob = scope.launch(start = CoroutineStart.UNDISPATCHED) {
            Log.d(
                "runRecomposeAndApplyChanges",
                "runRecomposeAndApplyChanges ${Thread.currentThread()}"
            )
            reComposer.runRecomposeAndApplyChanges()
        }
    }
    return rootNode.ref
}