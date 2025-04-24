package com.temple.skiaui.bitmap

import android.graphics.Bitmap
import android.graphics.drawable.Drawable
import android.os.Handler
import android.os.Looper
import androidx.core.graphics.drawable.toBitmap
import com.bumptech.glide.Glide
import com.bumptech.glide.load.engine.DiskCacheStrategy
import com.bumptech.glide.load.resource.gif.GifDrawable
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.transition.Transition
import com.temple.skiaui.HYSkiaEngine

class GlideImageLoader(val engine: HYSkiaEngine, val ref: Long) : ImageLoader {

    private val mainHandler = Handler(Looper.getMainLooper())

    private var bitmap: Bitmap? = null
    private var bitmapTarget: CustomTarget<Bitmap>? = null
    private var gifDrawable: GifDrawable? = null
    private var gifTarget: CustomTarget<GifDrawable>? = null
    private var released = false

    private var resId: Int = 0
    private var source: String? = null

    override fun requestBitmap(source: String) {
        this.source = source
        this.resId = 0
        mainHandler.post {
            innerRecycle()
            if (source.endsWith(".gif")) {
                processGif()
            } else {
                processBitmap()
            }
        }
    }

    override fun requestDrawable(resId: Int) {
        this.source = null
        this.resId = resId
        mainHandler.post {
            innerRecycle()
            processBitmap()
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
        mainHandler.post {
            released = true
            innerRecycle()
        }
    }

    override fun invalidateDrawable(drawable: Drawable) {
        if (released) {
            return
        }
        drawable.toBitmap(drawable.intrinsicWidth, drawable.intrinsicHeight)
            .apply {
                engine.updateAndroidBitmap(ref, this)
            }
    }

    private fun processGif() {
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
        builder.diskCacheStrategy(DiskCacheStrategy.RESOURCE).into(gifTarget!!)
    }

    private fun processBitmap() {
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
                if (released) {
                    return
                }
                this@GlideImageLoader.bitmap = bitmap
                engine.updateAndroidBitmap(ref, bitmap)
            }

            override fun onLoadCleared(placeholder: Drawable?) {}
        }
        bitmapTarget = target
        builder.diskCacheStrategy(DiskCacheStrategy.RESOURCE).into(bitmapTarget!!)
    }

    private fun isAbsoluteUrl(source: String? = null): Boolean {
        return source?.startsWith("http://") == true ||
                source?.startsWith("https://") == true ||
                source?.startsWith("file://") == true
    }

    private fun innerRecycle() {
        Glide.with(engine.getContext()).clear(gifTarget)
        Glide.with(engine.getContext()).clear(bitmapTarget)
        bitmap = null
        gifDrawable?.callback = null
        gifDrawable = null
    }

}
