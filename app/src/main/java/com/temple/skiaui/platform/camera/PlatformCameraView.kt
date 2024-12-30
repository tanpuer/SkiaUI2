package com.temple.skiaui.platform.camera

import android.annotation.SuppressLint
import android.content.Context
import android.content.Context.CAMERA_SERVICE
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.PorterDuff
import android.hardware.camera2.CameraAccessException
import android.hardware.camera2.CameraCaptureSession
import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CameraManager
import android.hardware.camera2.CameraMetadata
import android.hardware.camera2.CaptureRequest
import android.util.AttributeSet
import android.view.Surface
import android.view.View
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.platform.ICanvasProvider

class PlatformCameraView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    private var render: ICanvasProvider? = null

    private var cameraManager: CameraManager? = null

    private var cameraId: String = ""

    private var cameraDevice: CameraDevice? = null

    private var session: CameraCaptureSession? = null

    private var started = false

    fun setCanvasProvider(render: ICanvasProvider) {
        this.render = render
    }

    override fun onDraw(canvas: Canvas) {
        val glCanvas = render?.lockCanvas(canvas)
        if (glCanvas == null) {
            invalidate()
            return
        }
        glCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
        super.onDraw(glCanvas)
        render?.unLockCanvas(glCanvas)

        render?.getSurface()?.let {
            if (!started) {
                started = true
                startCamera()
                openCamera(it)
            }
        }
    }

    private fun startCamera() {
        cameraManager = HYSkiaUIApp.getInstance().getSystemService(CAMERA_SERVICE) as CameraManager
        cameraId = cameraManager?.cameraIdList?.getOrNull(0) ?: ""
    }

    @SuppressLint("MissingPermission")
    private fun openCamera(surface: Surface) {
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
                        this@PlatformCameraView.session = session
                        updatePreview(builder)
                    }

                    override fun onConfigureFailed(session: CameraCaptureSession) {

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
}