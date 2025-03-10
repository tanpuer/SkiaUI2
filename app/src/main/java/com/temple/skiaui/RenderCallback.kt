package com.temple.skiaui

interface RenderCallback {

    fun updateFps(draw: Int, render: Int)

    fun onPlatformBackPressed()
    
}