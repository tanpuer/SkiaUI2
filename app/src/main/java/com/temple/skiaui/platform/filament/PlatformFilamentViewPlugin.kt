package com.temple.skiaui.platform.filament

import android.animation.ValueAnimator
import android.graphics.SurfaceTexture
import android.opengl.Matrix
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.MotionEvent
import android.view.animation.LinearInterpolator
import com.google.android.filament.Box
import com.google.android.filament.Camera
import com.google.android.filament.Engine
import com.google.android.filament.Entity
import com.google.android.filament.EntityManager
import com.google.android.filament.Filament
import com.google.android.filament.IndexBuffer
import com.google.android.filament.Material
import com.google.android.filament.RenderableManager
import com.google.android.filament.Renderer
import com.google.android.filament.Scene
import com.google.android.filament.Skybox
import com.google.android.filament.SwapChain
import com.google.android.filament.VertexBuffer
import com.google.android.filament.View
import com.google.android.filament.Viewport
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.platform.SurfaceTextureBasePlugin
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.channels.Channels
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.sin

class PlatformFilamentViewPlugin(engine: HYSkiaEngine, width: Int, height: Int, viewPtr: Long) :
    SurfaceTextureBasePlugin(engine, width, height, viewPtr) {

    private lateinit var filamentEngine: Engine

    private lateinit var renderer: Renderer
    private lateinit var scene: Scene
    private lateinit var camera: Camera
    private lateinit var view: View

    private lateinit var material: Material
    private lateinit var vertexBuffer: VertexBuffer
    private lateinit var indexBuffer: IndexBuffer
    private var swapChain: SwapChain? = null

    // Filament entity representing a renderable object
    @Entity
    private var renderable = 0

    private val animator = ValueAnimator.ofFloat(0.0f, 360.0f)

    init {
        pluginHandler.post {
            createSurface()
            if (swapChain != null) {
                return@post
            }
            surfaceObj?.surface?.let {
                initEngine()
                swapChain = filamentEngine.createSwapChain(it)
            }
        }
    }

    override fun skiaSurfaceCreated() {
        super.skiaSurfaceCreated()
    }

    /**
     * TODO
     */
    override fun skiaSurfaceDestroyed() {
        surfaceObj?.release()
        surfaceObj = null
        pluginHandler.post {
            if (swapChain == null) {
                return@post
            }
            animator.cancel()
            // Cleanup all resources
            filamentEngine.destroyEntity(renderable)
            filamentEngine.destroyRenderer(renderer)
            filamentEngine.destroyVertexBuffer(vertexBuffer)
            filamentEngine.destroyIndexBuffer(indexBuffer)
            filamentEngine.destroyMaterial(material)
            filamentEngine.destroyView(view)
            filamentEngine.destroyScene(scene)
            filamentEngine.destroyCameraComponent(camera.entity)
            val entityManager = EntityManager.get()
            entityManager.destroy(renderable)
            entityManager.destroy(camera.entity)
            filamentEngine.destroy()
            swapChain = null
        }
    }

    override fun type(): String = "FilamentView"

    private fun initEngine() {
        setupFilament()
        setupView()
        setupScene()
        view.viewport = Viewport(0, 0, width, height)
    }

    override fun drawOneFrame(frameTimeNanos: Long) {
        pluginHandler.post {
            swapChain?.let {
                if (renderer.beginFrame(it, System.currentTimeMillis())) {
                    renderer.render(view)
                    renderer.endFrame()
                }
            }
        }
    }

    override fun dispatchTouchEvent(touchEvent: MotionEvent) {

    }

    override fun release() {
        skiaSurfaceDestroyed()
        super.release()
    }

    override fun onShow() {
        super.onShow()
        pluginHandler.post {
            createSurface()
            if (swapChain != null) {
                return@post
            }
            surfaceObj?.surface?.let {
                initEngine()
                swapChain = filamentEngine.createSwapChain(it)
            }
        }
    }

    override fun onHide() {
        super.onHide()
    }

    override fun onSizeChange(width: Int, height: Int) {
        super.onSizeChange(width, height)
        pluginHandler.post {
            view.viewport = Viewport(0, 0, width, height)
        }
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture?) {
        super.onFrameAvailable(surfaceTexture)
    }

    private fun setupFilament() {
        val config = Engine.Config()
        filamentEngine = Engine.Builder()
            .config(config)
            .featureLevel(Engine.FeatureLevel.FEATURE_LEVEL_0)
            .build()
        renderer = filamentEngine.createRenderer()
        scene = filamentEngine.createScene()
        view = filamentEngine.createView()
        camera = filamentEngine.createCamera(filamentEngine.entityManager.create())
    }

    private fun setupView() {
        scene.skybox = Skybox.Builder().color(0.035f, 0.035f, 0.035f, 1.0f).build(filamentEngine)
        // post-processing is not supported at feature level 0
        if (filamentEngine.activeFeatureLevel == Engine.FeatureLevel.FEATURE_LEVEL_0) {
            view.isPostProcessingEnabled = false
        }
        // Tell the view which camera we want to use
        view.camera = camera
        // Tell the view which scene we want to render
        view.scene = scene
    }

    private fun setupScene() {
        loadMaterial()
        createMesh()
        // To create a renderable we first create a generic entity
        renderable = EntityManager.get().create()
        // We then create a renderable component on that entity
        // A renderable is made of several primitives; in this case we declare only 1
        RenderableManager.Builder(1)
            // Overall bounding box of the renderable
            .boundingBox(Box(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.01f))
            // Sets the mesh data of the first primitive
            .geometry(0, RenderableManager.PrimitiveType.TRIANGLES, vertexBuffer, indexBuffer, 0, 3)
            // Sets the material of the first primitive
            .material(0, material.defaultInstance)
            .build(filamentEngine, renderable)

        // Add the entity to the scene to render it
        scene.addEntity(renderable)
        startAnimation()
    }

    private fun loadMaterial() {
        readUncompressedAsset("materials/baked_color.filamat").let {
            material = Material.Builder().payload(it, it.remaining()).build(filamentEngine)
            material.compile(
                Material.CompilerPriorityQueue.HIGH,
                Material.UserVariantFilterBit.ALL,
                Handler(Looper.getMainLooper())
            ) {
                Log.i("hellotriangle", "Material " + material.name + " compiled.")
            }
            filamentEngine.flush()
        }
    }

    private fun createMesh() {
        val intSize = 4
        val floatSize = 4
        val shortSize = 2
        // A vertex is a position + a color:
        // 3 floats for XYZ position, 1 integer for color
        val vertexSize = 3 * floatSize + intSize

        // Define a vertex and a function to put a vertex in a ByteBuffer
        data class Vertex(val x: Float, val y: Float, val z: Float, val color: Int)

        fun ByteBuffer.put(v: Vertex): ByteBuffer {
            putFloat(v.x)
            putFloat(v.y)
            putFloat(v.z)
            putInt(v.color)
            return this
        }

        // We are going to generate a single triangle
        val vertexCount = 3
        val a1 = PI * 2.0 / 3.0
        val a2 = PI * 4.0 / 3.0

        val vertexData = ByteBuffer.allocate(vertexCount * vertexSize)
            // It is important to respect the native byte order
            .order(ByteOrder.nativeOrder())
            .put(Vertex(1.0f, 0.0f, 0.0f, 0xffff0000.toInt()))
            .put(Vertex(cos(a1).toFloat(), sin(a1).toFloat(), 0.0f, 0xff00ff00.toInt()))
            .put(Vertex(cos(a2).toFloat(), sin(a2).toFloat(), 0.0f, 0xff0000ff.toInt()))
            // Make sure the cursor is pointing in the right place in the byte buffer
            .flip()

        // Declare the layout of our mesh
        vertexBuffer = VertexBuffer.Builder()
            .bufferCount(1)
            .vertexCount(vertexCount)
            // Because we interleave position and color data we must specify offset and stride
            // We could use de-interleaved data by declaring two buffers and giving each
            // attribute a different buffer index
            .attribute(
                VertexBuffer.VertexAttribute.POSITION,
                0,
                VertexBuffer.AttributeType.FLOAT3,
                0,
                vertexSize
            )
            .attribute(
                VertexBuffer.VertexAttribute.COLOR,
                0,
                VertexBuffer.AttributeType.UBYTE4,
                3 * floatSize,
                vertexSize
            )
            // We store colors as unsigned bytes but since we want values between 0 and 1
            // in the material (shaders), we must mark the attribute as normalized
            .normalized(VertexBuffer.VertexAttribute.COLOR)
            .build(filamentEngine)

        // Feed the vertex data to the mesh
        // We only set 1 buffer because the data is interleaved
        vertexBuffer.setBufferAt(filamentEngine, 0, vertexData)

        // Create the indices
        val indexData = ByteBuffer.allocate(vertexCount * shortSize)
            .order(ByteOrder.nativeOrder())
            .putShort(0)
            .putShort(1)
            .putShort(2)
            .flip()

        indexBuffer = IndexBuffer.Builder()
            .indexCount(3)
            .bufferType(IndexBuffer.Builder.IndexType.USHORT)
            .build(filamentEngine)
        indexBuffer.setBuffer(filamentEngine, indexData)
    }

    private fun startAnimation() {
        // Animate the triangle
        animator.interpolator = LinearInterpolator()
        animator.duration = 10000
        animator.repeatMode = ValueAnimator.RESTART
        animator.repeatCount = ValueAnimator.INFINITE
        animator.addUpdateListener(object : ValueAnimator.AnimatorUpdateListener {
            val transformMatrix = FloatArray(16)
            override fun onAnimationUpdate(a: ValueAnimator) {
                if (released) {
                    return
                }
                Matrix.setRotateM(transformMatrix, 0, -(a.animatedValue as Float), 0.0f, 0.0f, 1.0f)
                val tcm = filamentEngine.transformManager
                tcm.setTransform(tcm.getInstance(renderable), transformMatrix)
            }
        })
        animator.start()
    }

    private fun readUncompressedAsset(assetName: String): ByteBuffer {
        HYSkiaUIApp.getInstance().assets.openFd(assetName).use { fd ->
            val input = fd.createInputStream()
            val dst = ByteBuffer.allocate(fd.length.toInt())
            val src = Channels.newChannel(input)
            src.read(dst)
            src.close()
            return dst.apply { rewind() }
        }
    }

    companion object {
        init {
            Filament.init()
        }
    }

}