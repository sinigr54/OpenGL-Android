package com.example.sinigr.openglcourse.glcore.renderer

import android.opengl.GLES31
import android.opengl.GLES32
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GlRenderer : GLSurfaceView.Renderer {
    companion object {
        private val TAG = GlRenderer::class.java.simpleName
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES31.glClear(GLES31.GL_COLOR_BUFFER_BIT or GLES31.GL_DEPTH_BUFFER_BIT)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES31.glViewport(0, 0, width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES31.glClearColor(0.0f, 1.0f, 0.0f, 0.0f)
    }
}