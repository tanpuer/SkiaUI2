package com.temple.skiaui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.ViewGroup
import androidx.constraintlayout.widget.ConstraintLayout

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        addSkiaView()
    }

    private fun addSkiaView() {
        findViewById<ConstraintLayout>(R.id.clContainer).addView(
            HYSkiaSurfaceView(this),
//            HYSkiaTextureView(this),
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
    }

}