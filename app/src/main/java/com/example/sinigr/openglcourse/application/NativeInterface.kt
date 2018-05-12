package com.example.sinigr.openglcourse.application

import android.app.Activity
import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLUtils
import android.util.Log
import java.io.IOException

object NativeInterface {
    private val TAG = NativeInterface::class.java.simpleName

    init {
        System.loadLibrary("glcore")
    }

    var assetManager: AssetManager? = null

    external fun createNativeApplication(assetManager: AssetManager,
                                         pathToInternalDir: String): Long

    external fun destroyNativeApplication(nativeApplication: Long)

    external fun onPause(nativeApplication: Long)

    external fun onResume(nativeApplication: Long, context: Context, activity: Activity)

    external fun onSurfaceCreated(nativeApplication: Long);

    external fun onDisplayGeometryChanged(nativeApplication: Long, displayRotation: Int, width: Int, height: Int);

    external fun onDrawFrame(nativeApplication: Long);

    /* Called from native code */
    @JvmStatic
    fun loadImage(imageName: String): Bitmap? {
        return try {
            BitmapFactory.decodeStream(assetManager?.open(imageName))
        } catch (e: IOException) {
            Log.e(TAG, "Cannot open image $imageName")
            null
        }

    }

    /* Called from native code */
    @JvmStatic
    fun loadTexture(target: Int, bitmap: Bitmap) {
        GLUtils.texImage2D(target, 0, bitmap, 0)
    }
}