#pragma once

#include "Log.h"
#include "AudioSynthesizer.h"
#include "OboeAudioPlayer.h"
#include "WavetableOscillator.h"
#include <cmath>

namespace audiosynthesizer {

    AudioSynthesizer::AudioSynthesizer()
            : _oscillator{std::make_shared<WavetableOscillator>(
            _wavetableFactory.getWaveTable(_currentWavetable),
            sampleRate)},
              _audioPlayer{std::make_unique<OboeAudioPlayer>(_oscillator, sampleRate)} {}

    AudioSynthesizer::~AudioSynthesizer() = default;

    void AudioSynthesizer::play() {
        std::lock_guard<std::mutex> lock(_mutex);
        const auto result = _audioPlayer->play();
        if (result == 0) {
            _isPlaying = true;
        } else {
            _isPlaying = false;
        }
        LOGD("Native play() called");
    }

    void AudioSynthesizer::stop() {
        std::lock_guard<std::mutex> lock(_mutex);

        _audioPlayer->stop();
        _isPlaying = false;
        LOGD("Native stop() called");
    }

    bool AudioSynthesizer::isPlaying() {
        LOGD("Native isPlaying() called");
        return _isPlaying;
    }

    void AudioSynthesizer::setFrequency(float frequencyInHz) {
        LOGD("Native setFrequency() calling with frequencyInHz: %.2f", frequencyInHz);
        _oscillator->setFrequency(frequencyInHz);
    }

    float dBtoAmplitude(float dB) {
        return std::pow(10.f, dB / 20.f);
    }

    void AudioSynthesizer::setVolume(float volumeInDb) {
        const auto amplitude = dBtoAmplitude(volumeInDb);
        LOGD("Native setVolume() calling with volumeInDb: %.2f amplitude: %2.f", volumeInDb,amplitude);
        _oscillator->setAmplitude(amplitude);

    }

    void AudioSynthesizer::setWavetable(Wavetable wavetable) {
        LOGD("Native setWavetable() calling with wavetable: %d", static_cast<int>(wavetable));
        if (_currentWavetable != wavetable) {
            _currentWavetable = wavetable;
            _oscillator->setWavetable(_wavetableFactory.getWaveTable(wavetable));
        }
    }
}