package com.temple.skiaui

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class HYSkiaExampleActivity : AppCompatActivity() {

    private lateinit var cppBtn: Button
    private lateinit var jsBtn: Button
    private lateinit var kotlinBtn: Button
    private lateinit var reactjsBtn: Button
    private lateinit var vuejsBtn: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_example)
        cppBtn = findViewById(R.id.btn_cpp)
        cppBtn.setOnClickListener {
            goToMainActivity(HYSkiaEngine.DEVELOPMENT_CPP)
        }
        jsBtn = findViewById(R.id.btn_js)
        jsBtn.setOnClickListener {
            goToMainActivity(HYSkiaEngine.DEVELOPMENT_JS)
        }
        kotlinBtn = findViewById(R.id.btn_kotlin)
        kotlinBtn.setOnClickListener {
            goToMainActivity(HYSkiaEngine.DEVELOPMENT_COMPOSE)
        }
        reactjsBtn = findViewById(R.id.btn_react)
        reactjsBtn.setOnClickListener {
            goToMainActivity(HYSkiaEngine.DEVELOPMENT_REACT)
        }
        vuejsBtn = findViewById(R.id.btn_vue)
        vuejsBtn.setOnClickListener {
            goToMainActivity(HYSkiaEngine.DEVELOPMENT_VUE)
        }
        requestPermissions(
            arrayOf(
                android.Manifest.permission.RECORD_AUDIO,
                android.Manifest.permission.CAMERA,
                android.Manifest.permission.READ_MEDIA_VIDEO,
                android.Manifest.permission.POST_NOTIFICATIONS,
            ), 101
        )
    }

    private fun goToMainActivity(exampleType: Int) {
        val intent = Intent(this, MainActivity::class.java)
        intent.putExtra("type", exampleType)
        startActivity(intent)
    }

}