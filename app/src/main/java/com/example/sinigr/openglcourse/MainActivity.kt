package com.example.sinigr.openglcourse

import android.hardware.display.DisplayManager
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.example.sinigr.openglcourse.application.NativeInterface
import com.example.sinigr.openglcourse.glcore.renderer.NativeRenderer
import com.example.sinigr.openglcourse.glcore.view.GLView

class MainActivity : AppCompatActivity() {

    companion object {
        private const val TAG = "MainActivity"
    }

    private lateinit var glView: GLView
    private lateinit var renderer: NativeRenderer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        renderer = NativeRenderer(this)
        glView = GLView(this, renderer)

        glView.nativeApplication = renderer.nativeApplication

        setContentView(glView)
    }

    override fun onPause() {
        super.onPause()

        renderer.onPause()
        glView.onPause()
    }

    override fun onResume() {
        super.onResume()

        renderer.onResume()
        glView.onResume()
    }
}
