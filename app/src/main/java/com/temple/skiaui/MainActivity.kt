package com.temple.skiaui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat

class MainActivity : AppCompatActivity(), RenderCallback {

    private lateinit var fpsView: TextView
    private lateinit var skiaView: ISkiaView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        HYSkiaUIApp.getInstance().setFrameRate(this)
        setContentView(R.layout.activity_main)
        val controller = WindowCompat.getInsetsController(window, window.decorView)
        controller.hide(WindowInsetsCompat.Type.statusBars())
        skiaView = findViewById(R.id.surfaceView)
        val type = intent.getIntExtra("type", 0)
        skiaView.initEngine(type)
        skiaView.setRenderCallback(this)
        fpsView = findViewById(R.id.tvFps)
        requestPermissions(arrayOf(android.Manifest.permission.RECORD_AUDIO), 101)
    }

    override fun onBackPressed() {
        skiaView.onBackPressed()
    }

    override fun updateFps(value: Int) {
        fpsView.text = "fps: $value"
    }

    override fun onPlatformBackPressed() {
        super.onBackPressed()
    }

    override fun onDestroy() {
        super.onDestroy()
        skiaView.release()
    }

}