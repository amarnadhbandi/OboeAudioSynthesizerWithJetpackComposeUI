#pragma once

namespace audiosynthesizer {
    class AudioSource {
    public:
        virtual ~AudioSource() = default;

        virtual float getSample() = 0;

        virtual void onPlaybackStopped() = 0;
    };
}