package com.demo.audiosynthesizer

import android.util.Log

/**
 * LoggingWaveTableSynthesizer used for testing data model to test UI screen without JNI
 */
class LoggingWaveTableSynthesizer : AudioSynthesizer {
    private val TAG = LoggingWaveTableSynthesizer::class.java.simpleName
    private var isPlaying = false
    override suspend fun play() {
        Log.d(TAG, " play() called")
        isPlaying = true
    }

    override suspend fun stop() {
        Log.d(TAG, " stop() called")
        isPlaying = false
    }

    override suspend fun isPlaying(): Boolean {
        Log.d(TAG, " isPlaying() called")
        return isPlaying
    }

    override suspend fun setFrequency(frequencyInHz: Float) {
        Log.d(TAG, " setFrequency() called with  $frequencyInHz Hz")
    }

    override suspend fun setVolume(volumeInDb: Float) {
        Log.d(TAG, " setVolume() called with  $volumeInDb Db")
    }

    override suspend fun setWaveTable(wave: Wavetable) {
        Log.d(TAG, " setWaveTable() called with  $wave")
    }
}