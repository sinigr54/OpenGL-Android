package com.example.sinigr.openglcourse.glcore.renderer

import android.opengl.GLSurfaceView
import com.example.sinigr.openglcourse.application.NativeInterface
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NativeRenderer : GLSurfaceView.Renderer {
    companion object {
        private val TAG = NativeRenderer::class.java.simpleName
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        NativeInterface.onSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        NativeInterface.onSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        NativeInterface.onDrawFrame()
    }
}