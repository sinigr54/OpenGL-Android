package com.example.sinigr.openglcourse

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.example.sinigr.openglcourse.application.NativeInterface
import com.example.sinigr.openglcourse.glcore.view.GLView

class MainActivity : AppCompatActivity() {

    private lateinit var glView: GLView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        NativeInterface.assetManager = assets
        NativeInterface.createNativeApplication(assets)

        glView = GLView(this)
        setContentView(glView)
    }

    override fun onPause() {
        super.onPause()

        NativeInterface.onPause()
        glView.onPause()
        glView.stop()
    }

    override fun onResume() {
        super.onResume()

        NativeInterface.onResume(applicationContext, this)
        glView.onResume()
        glView.start()
    }
}
