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
import android.media.Image.Plane
import android.util.Log
import android.view.Surface
import android.view.WindowManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.platform.PlatformImageReaderBasePlugin
import com.temple.skiaui.platform.data.ImageReaderYUVData
import java.nio.ByteBuffer

class PlatformCameraViewPlugin(engine: HYSkiaEngine, width: Int, height: Int) :
    PlatformImageReaderBasePlugin(engine, width, height) {

    private var cameraManager: CameraManager? = null

    private var cameraId: String = ""

    private var cameraDevice: CameraDevice? = null

    private var session: CameraCaptureSession? = null

    private var rotation = 0

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

    override fun copyYUVData(planes: Array<Plane>, width: Int, height: Int): ImageReaderYUVData {
        return CameraYUVData().apply {
            yData = ByteBuffer.allocateDirect(planes[0].buffer.capacity()).put(planes[0].buffer)
                .rewind() as ByteBuffer
            uData = ByteBuffer.allocateDirect(planes[1].buffer.capacity()).put(planes[1].buffer)
                .rewind() as ByteBuffer
            vData = ByteBuffer.allocateDirect(planes[2].buffer.capacity()).put(planes[2].buffer)
                .rewind() as ByteBuffer
            stride = planes[0].rowStride
            this.rotation = this@PlatformCameraViewPlugin.rotation
            this.width = width
            this.height = height
        }
    }

    override fun innerOnShow() {
        super.innerOnShow()
        if (cameraManager == null) {
            surface?.let {
                startCamera()
                openCamera(it)
            }
        }
    }

    override fun innerOnHide() {
        super.innerOnHide()
        if (cameraManager != null) {
            closeCamera()
        }
    }

    override fun onSurfaceCreated(surface: Surface?) {
        surface?.let {
            startCamera()
            openCamera(it)
        }
    }

    override fun onSurfaceDestroyed(surface: Surface?) {
        closeCamera()
    }

    private fun startCamera() {
        Log.d(TAG, "startCamera")
        cameraManager = HYSkiaUIApp.getInstance().getSystemService(CAMERA_SERVICE) as CameraManager
        cameraId = cameraManager?.cameraIdList?.getOrNull(0) ?: ""
        rotation = getCameraRotation(HYSkiaUIApp.getInstance(), cameraId)
    }

    private fun closeCamera() {
        Log.d(TAG, "closeCamera")
        session?.close()
        session = null
        cameraDevice?.close()
        cameraDevice = null
        cameraManager = null
    }

    @SuppressLint("MissingPermission")
    private fun openCamera(surface: Surface) {
        Log.d(TAG, "openCamera")
        cameraManager?.openCamera(cameraId, object : CameraDevice.StateCallback() {
            override fun onOpened(camera: CameraDevice) {
                cameraDevice = camera
                createCameraPreviewSession(surface)
            }

            override fun onDisconnected(camera: CameraDevice) {
                camera.close()
            }

            override fun onError(camera: CameraDevice, error: Int) {
                camera.close()
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

    fun getCameraRotation(context: Context, cameraId: String): Int {
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
        // 计算最终的预览角度
        return (sensorOrientation + displayRotation + 360) % 360
    }

    companion object {
        private const val TAG = "HYCamera"
    }
}
