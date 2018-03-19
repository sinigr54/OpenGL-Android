package com.example.sinigr.openglcourse.glcore.view

import android.content.Context
import android.opengl.GLSurfaceView
import com.example.sinigr.openglcourse.glcore.renderer.NativeRenderer

class GLView(context: Context) : GLSurfaceView(context) {
    companion object {
        private val TAG = GLView::class.java.simpleName

        private const val GL_VERSION = 3
    }

    private val renderer = NativeRenderer()

    init {
        setEGLContextClientVersion(GL_VERSION)

        setRenderer(renderer)

        renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
    }
}