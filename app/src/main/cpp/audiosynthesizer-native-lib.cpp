#include <jni.h>
#include <memory>
#include "Log.h"
#include "AudioSynthesizer.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_create(JNIEnv *env, jobject thiz) {

    auto synthesizer = std::make_unique<audiosynthesizer::AudioSynthesizer>();

    if (not synthesizer) {
        LOGD("Failed to create the synthesizer");
        synthesizer.reset(nullptr);
    }
    return reinterpret_cast<jlong> (synthesizer.release());
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_delete(JNIEnv *env, jobject thiz,
                                                             jlong synthesizer_handle) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (not synthesizer) {
        LOGD("Attempt to destroy an unintialized synthesizer.");
        return;
    }
    delete (synthesizer);
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_play(JNIEnv *env, jobject thiz,
                                                           jlong synthesizer_handle) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (synthesizer) {
        synthesizer->play();
    } else {
        LOGD("Failed to play() AudioSynthesizer");
    }
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_stop(JNIEnv *env, jobject thiz,
                                                           jlong synthesizer_handle) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (synthesizer) {
        synthesizer->stop();
    } else {
        LOGD("Failed to stop() AudioSynthesizer");
    }
}

JNIEXPORT jboolean JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_isPlaying(JNIEnv *env, jobject thiz,
                                                                jlong synthesizer_handle) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (synthesizer) {
        return synthesizer->isPlaying();
    } else {
        LOGD("Failed to get isPlaying() from AudioSynthesizer");
    }
    return false;
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_setFrequency(JNIEnv *env, jobject thiz,
                                                                   jlong synthesizer_handle,
                                                                   jfloat frequency_in_hz) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (synthesizer) {
        synthesizer->setFrequency(static_cast<float>(frequency_in_hz));
    } else {
        LOGD("Failed to setFrequency() in AudioSynthesizer ");
    }
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_setVolume(JNIEnv *env, jobject thiz,
                                                                jlong synthesizer_handle,
                                                                jfloat volume_in_db) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    if (synthesizer) {
        synthesizer->setVolume(static_cast<float>(volume_in_db));
    } else {
        LOGD("Failed to setVolume() in AudioSynthesizer");
    }
}

JNIEXPORT void JNICALL
Java_com_demo_audiosynthesizer_NativeAudioSynthesizer_setWaveTable(JNIEnv *env, jobject thiz,
                                                                   jlong synthesizer_handle,
                                                                   jint wave_table) {
    auto *synthesizer = reinterpret_cast<audiosynthesizer::AudioSynthesizer *>(synthesizer_handle);
    const auto nativeWavetable = static_cast<audiosynthesizer::Wavetable>(wave_table);

    if (synthesizer) {
        synthesizer->setWavetable(nativeWavetable);
    } else {
        LOGD("Failed to setWaveTable() in AudioSynthesizer");
    }
}

}