package com.example.sinigr.openglcourse

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.example.sinigr.openglcourse.glcore.view.GLView

class MainActivity : AppCompatActivity() {

    private lateinit var glView: GLView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        glView = GLView(this)
        setContentView(glView)
    }

    override fun onPause() {
        super.onPause()

        glView.onPause()
    }

    override fun onResume() {
        super.onResume()

        glView.onResume()
    }
}
