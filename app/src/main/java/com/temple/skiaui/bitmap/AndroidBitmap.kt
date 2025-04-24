package com.temple.skiaui.bitmap

import android.graphics.Bitmap
import android.graphics.drawable.Drawable
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.core.graphics.drawable.toBitmap
import com.bumptech.glide.Glide
import com.bumptech.glide.load.engine.DiskCacheStrategy
import com.bumptech.glide.load.resource.gif.GifDrawable
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.transition.Transition
import com.temple.skiaui.HYSkiaEngine

class AndroidBitmap(val engine: HYSkiaEngine, val ref: Long) {

    private val mainHandler = Handler(Looper.getMainLooper())
    private var bitmap: Bitmap? = null
    private var resource: GifDrawable? = null

    private var resId: Int = 0
    private var source: String? = null
    private var released = false

    fun setSource(source: String) {
        this.source = source
        this.resId = -1
        requestBitmap()
    }

    fun setResId(resId: Int) {
        this.source = null
        this.resId = resId
        requestBitmap()
    }

    fun release() {
        mainHandler.post {
            resource?.callback = null
            released = true
            bitmap?.recycle()
            resource?.recycle()
        }
    }

    private fun requestBitmap() {
        mainHandler.post {
            if (source?.endsWith(".gif") == true) {
                processGif()
            } else {
                processPNG()
            }
        }
    }

    private val callback = object : Drawable.Callback {
        override fun invalidateDrawable(drawable: Drawable) {
            if (released) {
                return
            }
            drawable.toBitmap(drawable.intrinsicWidth, drawable.intrinsicHeight)
                .apply {
                    engine.updateAndroidBitmap(ref, this)
                }
        }

        override fun scheduleDrawable(who: Drawable, what: Runnable, `when`: Long) {
        }

        override fun unscheduleDrawable(who: Drawable, what: Runnable) {
        }

    }

    private fun processGif() {
        var builder = Glide.with(engine.getContext())
            .asGif()
        if (source != null) {
            if (source?.startsWith("http://") == true || source?.startsWith("https://") == true) {
                builder = builder.load(source)
            } else {
                builder = builder.load("file:///android_asset/${this.source}")
            }
        } else if (resId > 0) {
            builder = builder.load(resId)
        }
        builder.diskCacheStrategy(DiskCacheStrategy.RESOURCE)
            .into(object : CustomTarget<GifDrawable>() {
                override fun onResourceReady(
                    resource: GifDrawable,
                    transition: Transition<in GifDrawable>?
                ) {
                    if (released) {
                        return
                    }
                    resource.setLoopCount(GifDrawable.LOOP_FOREVER)
                    //new WeakReference<>(cb)!
                    resource.callback = callback
                    resource.start()
                }

                override fun onLoadCleared(placeholder: Drawable?) {}
            })
    }

    private fun processPNG() {
        var builder = Glide.with(engine.getContext())
            .asBitmap()
        if (source != null) {
            if (source?.startsWith("http://") == true || source?.startsWith("https://") == true) {
                builder = builder.load(source)
            } else {
                builder = builder.load("file:///android_asset/${this.source}")
            }
        } else if (resId > 0) {
            builder = builder.load(resId)
        }
        builder.diskCacheStrategy(DiskCacheStrategy.RESOURCE)
            .into(object : CustomTarget<Bitmap>() {
                override fun onResourceReady(
                    resource: Bitmap,
                    transition: Transition<in Bitmap>?
                ) {
                    if (released) {
                        return
                    }
                    bitmap = resource
                    engine.postToSkiaUI {
                        engine.updateAndroidBitmap(ref, resource)
                    }
                }

                override fun onLoadCleared(placeholder: Drawable?) {}
            })
    }

    private fun start() {
        mainHandler.post {
            resource?.start()
        }
    }

    private fun stop() {
        mainHandler.post {
            resource?.stop()
        }
    }

    companion object {
        private const val TAG = "AndroidBitmap"
    }

}