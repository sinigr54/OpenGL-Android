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

        val internalDirectory = filesDir.absolutePath

        NativeInterface.assetManager = assets

        val nativeApplication = NativeInterface.createNativeApplication(assets, internalDirectory)
        renderer = NativeRenderer(nativeApplication, this)

        glView = GLView(this, renderer)
        setContentView(glView)
    }

    override fun onPause() {
        super.onPause()

        NativeInterface.onPause(renderer.nativeApplication)
        glView.onPause()
        glView.stop()
    }

    override fun onResume() {
        super.onResume()

        NativeInterface.onResume(renderer.nativeApplication, applicationContext, this)
        glView.onResume()
        glView.start()
    }
}
