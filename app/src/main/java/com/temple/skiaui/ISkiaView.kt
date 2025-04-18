package com.temple.skiaui

interface ISkiaView {

    fun initEngine(type: Int)

    fun setRenderCallback(renderCallback: RenderCallback)

    fun onBackPressed()

    fun release()

    fun onUIModeChange()
    
}