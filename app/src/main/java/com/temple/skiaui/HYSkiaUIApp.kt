package com.temple.skiaui

import android.app.Activity
import android.app.Application
import android.os.Build

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

        private var globalFrameRate = 60
    }

    fun setFrameRate(activity: Activity) {
        globalFrameRate = if (Build.VERSION.SDK_INT >= 30) {
            activity.display?.mode?.refreshRate?.toInt() ?: 60
        } else {
            60
        }
    }

    fun getFrameRate(): Int {
        return globalFrameRate
    }

}