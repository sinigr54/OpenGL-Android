package com.example.sinigr.openglcourse.glcore.view

import android.annotation.SuppressLint
import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import android.view.MotionEvent
import com.example.sinigr.openglcourse.glcore.renderer.NativeRenderer

class GLView(context: Context) : GLSurfaceView(context) {
    companion object {
        private val TAG = GLView::class.java.simpleName

        private const val GL_VERSION = 3
    }

    private var running: Boolean = false
    private val renderer = NativeRenderer()
    private val gameLoop = Runnable {
        while (running) {
            try {
                Thread.sleep(100)
                requestRender()
            } catch (e: InterruptedException) {

            }
        }
    }

    private lateinit var gameThread: Thread

    init {
        setEGLContextClientVersion(GL_VERSION)

        setRenderer(renderer)

        renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent?): Boolean {
        when (event?.action) {
            MotionEvent.ACTION_MOVE -> {
                requestRender()
            }
        }

        return true
    }

    fun start() {
        running = true

        gameThread = Thread(gameLoop)
        gameThread.start()
    }

    fun stop() {
        running = false

        gameThread.interrupt()
        gameThread.join()
    }
}