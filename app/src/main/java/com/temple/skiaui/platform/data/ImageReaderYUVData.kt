package com.temple.skiaui.platform.data

import java.nio.ByteBuffer

open class ImageReaderYUVData {

    lateinit var yData: ByteBuffer

    lateinit var uData: ByteBuffer

    lateinit var vData: ByteBuffer

    var width = 0

    var height = 0

    var stride = 0

    var rotation = 0
}