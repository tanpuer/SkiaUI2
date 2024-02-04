package com.temple.skiaui

import android.app.Application

class HYSkiaUIApp : Application() {

    override fun onCreate() {
        super.onCreate()
        mInstance = this
    }

    companion object {
        private lateinit var mInstance: HYSkiaUIApp

        fun getInstance(): HYSkiaUIApp {
            return mInstance
        }
    }
}