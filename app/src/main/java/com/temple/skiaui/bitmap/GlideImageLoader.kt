package com.temple.skiaui.bitmap

import android.app.Activity
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.Drawable
import com.bumptech.glide.Glide
import com.bumptech.glide.load.engine.DiskCacheStrategy
import com.bumptech.glide.load.resource.gif.GifDrawable
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.transition.Transition
import com.temple.skiaui.HYSkiaEngine
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class GlideImageLoader(val engine: HYSkiaEngine, val ref: Long) : ImageLoader {

    private val mainHandler = engine.mainHandler

    private var bitmap: Bitmap? = null
    private var bitmapTarget: CustomTarget<Bitmap>? = null
    private var gifDrawable: GifDrawable? = null
    private var gifTarget: CustomTarget<GifDrawable>? = null

    @Volatile
    private var released = false

    private var resId: Int = 0
    private var source: String? = null

    override fun requestBitmap(source: String, viewWidth: Int, viewHeight: Int) {
        this.source = source
        this.resId = 0
        mainHandler.post {
            innerRecycle()
            if (source.endsWith(".gif")) {
                processGif(viewWidth, viewHeight)
            } else {
                processBitmap(viewWidth, viewHeight)
            }
        }
    }

    override fun requestDrawable(resId: Int, viewWidth: Int, viewHeight: Int) {
        this.source = null
        this.resId = resId
        mainHandler.post {
            innerRecycle()
            processBitmap(viewWidth, viewHeight)
        }
    }

    override fun start() {
        mainHandler.post {
            gifDrawable?.start()
        }
    }

    override fun stop() {
        mainHandler.post {
            gifDrawable?.stop()
        }
    }

    override fun release() {
        released = true
        mainHandler.post {
            gifDrawable?.stop()
            innerRecycle()
        }
    }

    override fun invalidateDrawable(drawable: Drawable) {
        CoroutineScope(Dispatchers.IO).launch {
            drawable.toBitmap(drawable.intrinsicWidth, drawable.intrinsicHeight)
                .apply {
                    engine.postToSkiaUI {
                        if (released) {
                            return@postToSkiaUI
                        }
                        engine.updateAndroidBitmap(
                            ref,
                            this,
                            gifDrawable?.frameIndex ?: 0,
                            gifDrawable?.frameCount ?: 0
                        )
                    }
                }
        }
    }

    /**
     * Can not get bitmap from Glide.
     * Transfer drawable to Bitmap costing much time.
     * Must not block UI Thread.
     */
    private fun Drawable.toBitmap(width: Int, height: Int): Bitmap {
        val bitmapPool = Glide.get(engine.getContext()).bitmapPool
        val bitmap = bitmapPool.get(width, height, Bitmap.Config.ARGB_8888)
        val canvas = Canvas(bitmap)
        val bounds = this.bounds
        this.setBounds(0, 0, width, height)
        this.draw(canvas)
        this.bounds = bounds
        return bitmap
    }

    private fun processGif(viewWidth: Int, viewHeight: Int) {
        var builder = Glide.with(engine.getContext())
            .asGif()
        if (source != null) {
            builder = if (isAbsoluteUrl(source)) {
                builder.load(source)
            } else {
                builder.load("file:///android_asset/${source}")
            }
        } else if (resId > 0) {
            builder = builder.load(resId)
        }
        val target = object : CustomTarget<GifDrawable>() {
            override fun onResourceReady(
                gifDrawable: GifDrawable,
                transition: Transition<in GifDrawable>?
            ) {
                if (released) {
                    return
                }
                this@GlideImageLoader.gifDrawable = gifDrawable
                gifDrawable.setLoopCount(GifDrawable.LOOP_FOREVER)
                //new WeakReference<>(cb)!
                gifDrawable.callback = this@GlideImageLoader
                gifDrawable.start()
            }

            override fun onLoadCleared(placeholder: Drawable?) {}
        }
        gifTarget = target
        var loader = builder.diskCacheStrategy(DiskCacheStrategy.ALL)
        if (viewWidth > 0 && viewHeight > 0) {
            loader = loader.override(viewWidth, viewHeight)
        }
        loader.into(target)
    }

    private fun processBitmap(viewWidth: Int, viewHeight: Int) {
        var builder = Glide.with(engine.getContext())
            .asBitmap()
        if (source != null) {
            builder = if (isAbsoluteUrl(source)) {
                builder.load(source)
            } else {
                builder.load("file:///android_asset/${source}")
            }
        } else if (resId > 0) {
            builder = builder.load(resId)
        }
        val target = object : CustomTarget<Bitmap>() {
            override fun onResourceReady(
                bitmap: Bitmap,
                transition: Transition<in Bitmap>?
            ) {
                this@GlideImageLoader.bitmap = bitmap
                engine.postToSkiaUI {
                    if (released) {
                        return@postToSkiaUI
                    }
                    engine.updateAndroidBitmap(ref, bitmap, 0, 0)
                }
            }

            override fun onLoadCleared(placeholder: Drawable?) {}
        }
        bitmapTarget = target
        var loader = builder.diskCacheStrategy(DiskCacheStrategy.ALL)
        if (viewWidth > 0 && viewHeight > 0) {
            loader = loader.override(viewWidth, viewHeight)
        }
        loader.into(target)
    }

    private fun isAbsoluteUrl(source: String? = null): Boolean {
        return source?.startsWith("http://") == true ||
                source?.startsWith("https://") == true ||
                source?.startsWith("file://") == true
    }

    private fun innerRecycle() {
        val isFinishing = (engine.getContext() as Activity).isFinishing
        val isDestroyed = (engine.getContext() as Activity).isDestroyed
        if (!isFinishing || !isDestroyed) {
            Glide.with(engine.getContext()).clear(gifTarget)
            Glide.with(engine.getContext()).clear(bitmapTarget)
        }
        bitmap = null
        gifDrawable?.callback = null
        gifDrawable = null
    }

}
