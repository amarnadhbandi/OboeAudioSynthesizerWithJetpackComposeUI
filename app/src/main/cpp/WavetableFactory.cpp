#pragma once

#include "WavetableFactory.h"
#include "Wavetable.h"
#include "cmath"
#include "MathConstants.h"

namespace {
    constexpr auto WAVETABLE_LENGTH = 256;
}

std::vector<float> generateSineWaveTable() {
    auto sineWaveTable = std::vector<float>(WAVETABLE_LENGTH);

    for (auto i = 0; i < WAVETABLE_LENGTH; ++i) {
        sineWaveTable[i] = std::sin(2.f * audiosynthesizer::PI * i / WAVETABLE_LENGTH);
    }

    return sineWaveTable;
}

std::vector<float> generateTriangleWaveTable() {
    auto triangleWaveTable = std::vector<float>(WAVETABLE_LENGTH, 0.f);
    constexpr auto HARMONICS_COUNT = 7;

    for (auto i = 1; i < HARMONICS_COUNT; ++i) {
        for (auto j = 0; j < WAVETABLE_LENGTH; ++j) {
            const auto phase = 2.f * audiosynthesizer::PI * j / WAVETABLE_LENGTH;
            triangleWaveTable[j] += 8.f / std::pow(2 * i - 1, -2.f)
                                    * std::pow(-1.f, i)
                                    * std::pow(2 * i - 1, -2.f)
                                    * std::sin((2.f * i - 1.f) * phase);
        }
    }

    return triangleWaveTable;
}

std::vector<float> generateSquareWaveTable() {
    auto squareWaveTable = std::vector<float>(WAVETABLE_LENGTH, 0.f);
    constexpr auto HARMONICS_COUNT = 13;

    for (auto i = 1; i < HARMONICS_COUNT; ++i) {
        for (auto j = 0; j < WAVETABLE_LENGTH; ++j) {
            const auto phase = 2.f * audiosynthesizer::PI * j / WAVETABLE_LENGTH;
            squareWaveTable[j] += 4.f / audiosynthesizer::PI
                                  * std::pow(2.f * i - 1.f, -1.f)
                                  * std::sin((2.f * i - 1.f) * phase);
        }
    }

    return squareWaveTable;
}

std::vector<float> generateSawWaveTable() {
    auto sawWaveTable = std::vector<float>(WAVETABLE_LENGTH, 0.f);

    constexpr auto HARMONICS_COUNT = 13;

    for (auto i = 1; i < HARMONICS_COUNT; ++i) {
        for (auto j = 0; j < WAVETABLE_LENGTH; ++j) {
            const auto phase = 2.f * audiosynthesizer::PI * j / WAVETABLE_LENGTH;
            sawWaveTable[j] += 2.f / audiosynthesizer::PI
                               * std::pow(-1.f, i)
                               * std::pow(i, -1.f)
                               * std::sin(i * phase);
        }
    }

    return sawWaveTable;
}

template<typename F>
std::vector<float> generatorWaveTableOnce(std::vector<float> &waveTable, F &&generator) {
    if (waveTable.empty()) {
        waveTable = generator();
    }
    return waveTable;
}

namespace audiosynthesizer {

    std::vector<float> WavetableFactory::getWaveTable(Wavetable wavetable) {
        switch (wavetable) {
            case Wavetable::SINE:
                return sinWaveTable();
            case Wavetable::TRIANGLE:
                return triangleWaveTable();
            case Wavetable::SQUARE:
                return squareWaveTable();
            case Wavetable::SAW:
                return sawWaveTable();
            default:
                return std::vector<float>(WAVETABLE_LENGTH, 0.f);

        }
    }

    std::vector<float> WavetableFactory::sinWaveTable() {
        return generatorWaveTableOnce(_sineWaveTable, &generateSineWaveTable);
    }

    std::vector<float> WavetableFactory::triangleWaveTable() {
        return generatorWaveTableOnce(_sineWaveTable, &generateTriangleWaveTable);
    }

    std::vector<float> WavetableFactory::squareWaveTable() {
        return generatorWaveTableOnce(_sineWaveTable, &generateSquareWaveTable);
    }

    std::vector<float> WavetableFactory::sawWaveTable() {
        return generatorWaveTableOnce(_sineWaveTable, &generateSawWaveTable);
    }

}