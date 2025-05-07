package com.temple.skiaui

import android.content.res.Configuration
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.WindowManager.LayoutParams.SOFT_INPUT_ADJUST_NOTHING
import android.widget.TextView

class MainActivity : AppCompatActivity(), RenderCallback {

    private lateinit var fpsView: TextView
    private lateinit var skiaView: ISkiaView
    private var currentNightMode: Int = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        currentNightMode = resources.configuration.uiMode
        HYSkiaUIApp.getInstance().setFrameRate(this)
        setContentView(R.layout.activity_main)
        skiaView = findViewById(R.id.surfaceView)
        val type = intent.getIntExtra("type", 0)
        skiaView.initEngine(type)
        skiaView.setRenderCallback(this)
        fpsView = findViewById(R.id.tvFps)
        window.setSoftInputMode(SOFT_INPUT_ADJUST_NOTHING)
    }

    override fun onBackPressed() {
        skiaView.onBackPressed()
    }

    override fun updateFps(draw: Int, render: Int) {
        fpsView.text = "draw:$draw render:${render}"
    }

    override fun onPlatformBackPressed() {
        super.onBackPressed()
    }

    override fun onDestroy() {
        super.onDestroy()
        skiaView.release()
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        val newNightMode = newConfig.uiMode and Configuration.UI_MODE_NIGHT_MASK
        if (newNightMode != currentNightMode) {
            currentNightMode = newNightMode
            skiaView.onUIModeChange()
        }
    }

}