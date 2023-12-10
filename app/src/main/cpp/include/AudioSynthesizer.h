#pragma once

#include <memory>
#include <mutex>
#include "Wavetable.h"
#include "WavetableFactory.h"
#include "WavetableOscillator.h"

namespace audiosynthesizer {

    class WavetableOscillator;

    class AudioPlayer;

    constexpr auto sampleRate = 48000;

    class AudioSynthesizer {
    public:
        AudioSynthesizer();

        ~AudioSynthesizer();

        void play();

        void stop();

        bool isPlaying();

        void setFrequency(float frequencyInHz);

        void setVolume(float volumeInDb);

        void setWavetable(Wavetable wavetable);

    private:
        std::atomic<bool> _isPlaying = false;
        std::mutex _mutex;
        WavetableFactory _wavetableFactory;
        Wavetable _currentWavetable{Wavetable::SINE};
        std::shared_ptr<WavetableOscillator> _oscillator;
        std::unique_ptr<AudioPlayer> _audioPlayer;

    };
}