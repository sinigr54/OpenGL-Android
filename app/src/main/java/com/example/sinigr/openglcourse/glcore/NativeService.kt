package com.example.sinigr.openglcourse.glcore

class NativeService {
    companion object {
        init {
            System.loadLibrary("glcore")
        }
    }

    external fun stringFromJNI(): String
}