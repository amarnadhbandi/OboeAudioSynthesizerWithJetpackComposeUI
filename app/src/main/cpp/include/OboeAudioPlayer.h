#pragma once

#include <memory>
#include <oboe/Oboe.h>
#include "AudioPlayer.h"
#include "AudioSource.h"

namespace audiosynthesizer {
    class AudioSource;

    class OboeAudioPlayer : public oboe::AudioStreamDataCallback,
                            public AudioPlayer {
    public:
        static constexpr auto channelCount = oboe::ChannelCount::Mono;

        OboeAudioPlayer(std::shared_ptr<AudioSource> source, int samplingRate);

        ~OboeAudioPlayer();

        int32_t play() override;

        void stop() override;

        oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream,
                                              void *audioData,
                                              int32_t frameCount) override;

    private:
        std::shared_ptr<AudioSource> _source;
        std::shared_ptr<oboe::AudioStream> _stream;
        int _samplingRate;
    };
}