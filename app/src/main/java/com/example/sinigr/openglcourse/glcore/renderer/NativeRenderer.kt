package com.example.sinigr.openglcourse.glcore.renderer

import android.opengl.GLES31
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NativeRenderer : GLSurfaceView.Renderer {
    companion object {
        private val TAG = NativeRenderer::class.java.simpleName

        init {
            System.loadLibrary("glcore")
        }
    }

    private external fun nativeOnSurfaceCreated();

    private external fun nativeOnSurfaceChanged(width: Int, height: Int);

    private external fun nativeOnDrawFrame();

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        nativeOnDrawFrame()
    }
}