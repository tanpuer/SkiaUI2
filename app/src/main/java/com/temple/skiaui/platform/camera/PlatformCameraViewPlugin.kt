package com.temple.skiaui.platform.camera

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.content.Context.CAMERA_SERVICE
import android.content.pm.PackageManager
import android.hardware.camera2.CameraAccessException
import android.hardware.camera2.CameraCaptureSession
import android.hardware.camera2.CameraCharacteristics
import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CameraManager
import android.hardware.camera2.CameraMetadata
import android.hardware.camera2.CaptureRequest
import android.util.Log
import android.util.Size
import android.view.MotionEvent
import android.view.Surface
import android.view.WindowManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.platform.SurfaceTextureBasePlugin

class PlatformCameraViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, width, height, viewPtr) {

    private var cameraManager: CameraManager? = null

    private var cameraId: String = ""

    private var cameraDevice: CameraDevice? = null

    private var session: CameraCaptureSession? = null

    private var isBackCamera = true

    @Volatile
    private var rotation = -1
    @Volatile
    private var cameraWidth = 0
    @Volatile
    private var cameraHeight = 0

    private var opened = false

    init {
        if (ContextCompat.checkSelfPermission(engine.view.context, Manifest.permission.CAMERA)
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                engine.view.context as Activity,
                arrayOf(Manifest.permission.CAMERA),
                100
            )
        }
    }

    override fun skiaSurfaceCreated() {
        start()
    }

    override fun skiaSurfaceDestroyed() {
        if (cameraManager != null) {
            closeCamera()
        }
    }

    override fun type(): String = "CameraView"

    override fun dispatchTouchEvent(touchEvent: MotionEvent) {
    }

    override fun drawOneFrame(frameTimeNanos: Long) {

    }

    fun start() {
        pluginHandler.post {
            if (cameraManager == null) {
                startCamera()
            }
            if (surfaceObj?.surface == null) {
                //create surfaceTexture with camera size
                createSurface(cameraWidth, cameraHeight)
            }
            val surface = surfaceObj?.surface
            if (surface == null) {
                Log.e(TAG, "SurfaceObj is null!")
                return@post
            }
            if (!opened) {
                openCamera(surface)
            }
        }
    }

    override fun onShow() {
        super.onShow()
        start()
    }

    override fun onHide() {
        super.onHide()
        pluginHandler.post {
            skiaSurfaceDestroyed()
        }
    }

    private fun getRotation(): Int {
        return rotation
    }

    private fun getCameraWidth(): Int {
        return cameraWidth
    }

    private fun getCameraHeight(): Int {
        return cameraHeight
    }

    private fun switchCamera() {
        pluginHandler.post {
            isBackCamera = !isBackCamera
            closeCamera()
            start()
        }
    }

    private fun startCamera() {
        Log.d(TAG, "startCamera")
        cameraManager = HYSkiaUIApp.getInstance().getSystemService(CAMERA_SERVICE) as CameraManager
        cameraId = cameraManager?.cameraIdList?.getOrNull(if (isBackCamera) 0 else 1) ?: ""
        val rotation = getCameraRotation(HYSkiaUIApp.getInstance(), cameraId)
        val size = getCameraSize(HYSkiaUIApp.getInstance(), cameraId)
        this.rotation = rotation
        this.cameraWidth = size.width
        this.cameraHeight = size.height
    }

    private fun closeCamera() {
        Log.d(TAG, "closeCamera")
        session?.close()
        session = null
        cameraDevice?.close()
        cameraDevice = null
        cameraManager = null
        opened = false
    }

    @SuppressLint("MissingPermission")
    private fun openCamera(surface: Surface) {
        Log.d(TAG, "openCamera")
        opened = true
        cameraManager?.openCamera(cameraId, object : CameraDevice.StateCallback() {
            override fun onOpened(camera: CameraDevice) {
                cameraDevice = camera
                createCameraPreviewSession(surface)
            }

            override fun onDisconnected(camera: CameraDevice) {
                camera.close()
                opened = false
            }

            override fun onError(camera: CameraDevice, error: Int) {
                camera.close()
                opened = false
            }
        }, null)
    }

    private fun createCameraPreviewSession(surface: Surface) {
        try {
            val builder =
                cameraDevice?.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW) ?: return
            builder.addTarget(surface)

            cameraDevice?.createCaptureSession(
                listOf(surface),
                object : CameraCaptureSession.StateCallback() {
                    override fun onConfigured(session: CameraCaptureSession) {
                        this@PlatformCameraViewPlugin.session = session
                        updatePreview(builder)
                        Log.d(TAG, "onConfigured")
                    }

                    override fun onConfigureFailed(session: CameraCaptureSession) {
                        Log.d(TAG, "onConfigureFailed")
                    }
                },
                null
            )
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    private fun updatePreview(builder: CaptureRequest.Builder) {
        builder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO)
        try {
            session?.setRepeatingRequest(builder.build(), null, null)
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    private fun getCameraRotation(context: Context, cameraId: String): Int {
        // 获取 WindowManager
        val windowManager = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        val rotation = windowManager.defaultDisplay.rotation
        // 获取 CameraManager
        val cameraManager = context.getSystemService(Context.CAMERA_SERVICE) as CameraManager
        val characteristics = cameraManager.getCameraCharacteristics(cameraId)
        // 获取传感器方向
        val sensorOrientation = characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION) ?: 0
        // 计算预览的角度
        val displayRotation = when (rotation) {
            Surface.ROTATION_0 -> 0
            Surface.ROTATION_90 -> 90
            Surface.ROTATION_180 -> 180
            Surface.ROTATION_270 -> 270
            else -> 0
        }
        return (sensorOrientation + displayRotation + 360) % 360
    }

    private fun getCameraSize(context: Context, cameraId: String): Size {
        val cameraManager = context.getSystemService(Context.CAMERA_SERVICE) as CameraManager
        val characteristics = cameraManager.getCameraCharacteristics(cameraId)
        return characteristics.get(CameraCharacteristics.SENSOR_INFO_PIXEL_ARRAY_SIZE)
            ?: Size(width, height)
    }

    companion object {
        private const val TAG = "HYCamera"
    }
}
