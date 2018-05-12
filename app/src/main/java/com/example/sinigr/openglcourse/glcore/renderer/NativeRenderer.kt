package com.example.sinigr.openglcourse.glcore.renderer

import android.app.Activity
import android.content.Context
import android.hardware.display.DisplayManager
import android.opengl.GLSurfaceView
import android.util.Log
import com.example.sinigr.openglcourse.application.NativeInterface
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NativeRenderer(val nativeApplication: Long, private val activity: Activity?)
    : GLSurfaceView.Renderer, DisplayManager.DisplayListener {

    companion object {
        private val TAG = "NativeRenderer"
    }

    private var isViewportChanged = false
    private var viewportWidth: Int = 0
    private var viewportHeight: Int = 0

    init {
        /*
            Listen to display changed events to detect 180° rotation, which does not cause a config
            change or view resize.
        */
        activity?.getSystemService(DisplayManager::class.java)
                ?.registerDisplayListener(this, null)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.w(TAG, "onSurfaceCreated")

        NativeInterface.onSurfaceCreated(nativeApplication)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.w(TAG, "onSurfaceChanged")

        isViewportChanged = true
        viewportWidth = width
        viewportHeight = height
    }

    override fun onDrawFrame(gl: GL10?) {
        synchronized(this) {
            if (nativeApplication == 0L) {
                return
            }

            if (isViewportChanged) {
                isViewportChanged = false

                val displayRotation: Int = activity?.windowManager?.defaultDisplay?.rotation ?: 0
                NativeInterface.onDisplayGeometryChanged(nativeApplication, displayRotation, viewportWidth, viewportHeight)
            }

            NativeInterface.onDrawFrame(nativeApplication)
        }
    }

    override fun onDisplayChanged(displayId: Int) {
        isViewportChanged = true
    }

    override fun onDisplayAdded(displayId: Int) {

    }

    override fun onDisplayRemoved(displayId: Int) {

    }
}