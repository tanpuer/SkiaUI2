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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_example)
        cppBtn = findViewById(R.id.btn_cpp)
        cppBtn.setOnClickListener {
            goToMainActivity(0)
        }
        jsBtn = findViewById(R.id.btn_js)
        jsBtn.setOnClickListener {
            goToMainActivity(1)
        }
        kotlinBtn = findViewById(R.id.btn_kotlin)
        kotlinBtn.setOnClickListener {
            goToMainActivity(2)
        }
        reactjsBtn = findViewById(R.id.btn_react)
        reactjsBtn.setOnClickListener {
            goToMainActivity(3)
        }
        requestPermissions(
            arrayOf(
                android.Manifest.permission.RECORD_AUDIO,
                android.Manifest.permission.CAMERA
            ), 101
        )
    }

    private fun goToMainActivity(exampleType: Int) {
        val intent = Intent(this, MainActivity::class.java)
        intent.putExtra("type", exampleType)
        startActivity(intent)
    }

}