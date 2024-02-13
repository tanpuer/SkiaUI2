package com.temple.skiaui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat

class MainActivity : AppCompatActivity(), RenderCallback {

    private lateinit var fpsView: TextView
    private lateinit var surfaceView: HYSkiaSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val controller = WindowCompat.getInsetsController(window, window.decorView)
        controller.hide(WindowInsetsCompat.Type.statusBars())
        surfaceView = findViewById(R.id.surfaceView)
        surfaceView.setRenderCallback(this)
        fpsView = findViewById(R.id.tvFps)
    }

    override fun updateFps(value: Int) {
        fpsView.text = "fps: $value"
    }

}