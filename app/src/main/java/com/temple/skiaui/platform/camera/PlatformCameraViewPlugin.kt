package com.temple.skiaui.platform.camera

import android.Manifest
import android.app.Activity
import android.content.pm.PackageManager
import android.view.View
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.platform.PlatformBasePlugin

class PlatformCameraViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, editTextPtr: Long) :
    PlatformBasePlugin(engine, width, height, editTextPtr) {

    override fun initPlatformView(): View {
        if (ContextCompat.checkSelfPermission(engine.view.context, Manifest.permission.CAMERA)
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                engine.view.context as Activity,
                arrayOf(Manifest.permission.CAMERA),
                100
            )
        }
        val cameraView = PlatformCameraView(engine.view.context)
        cameraView.setCanvasProvider(this)
        return cameraView
    }

    override fun destroyPlatformView() {

    }
}