package com.temple.skiaui.platform.data

import java.nio.ByteBuffer

open class ImageReaderYUVData {

    lateinit var yData: ByteBuffer

    lateinit var uData: ByteBuffer

    lateinit var vData: ByteBuffer

    var width = 0

    var height = 0

    var strideY = 0

    var strideU = 0

    var strideV = 0

    var rotation = 0
}