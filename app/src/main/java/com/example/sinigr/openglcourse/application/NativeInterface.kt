package com.example.sinigr.openglcourse.application

import android.app.Activity
import android.content.Context
import android.content.res.AssetManager

object NativeInterface {
    private val TAG = NativeInterface::class.java.simpleName

    init {
        System.loadLibrary("glcore")
    }

    external fun createNativeApplication(assetManager: AssetManager)

    external fun destroyNativeApplication()

    external fun onPause()

    external fun onResume(context: Context, activity: Activity)

    external fun onSurfaceCreated();

    external fun onSurfaceChanged(width: Int, height: Int);

    external fun onDrawFrame();
}