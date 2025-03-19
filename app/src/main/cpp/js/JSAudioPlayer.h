#pragma once

#include "SkiaUIContext.h"

namespace HYSkiaUI {

class JSAudioPlayer {

public:

    JSAudioPlayer(std::shared_ptr<SkiaUIContext> &context, std::string& path);

    ~JSAudioPlayer();

    void start();

    void pause();

    void release();

    long getCurrPosition();

    long getDuration();

    void seek(long position);

private:

    std::shared_ptr<SkiaUIContext> context = nullptr;

    jclass javaAudioPlayerClass = nullptr;

    jmethodID javaAudioPlayerConstructor = nullptr;

    jmethodID getFFTDataMethodID = nullptr;

    jmethodID startMethodID = nullptr;

    jmethodID pauseMethodID = nullptr;

    jmethodID currentPositionMethodID = nullptr;

    jmethodID releaseMethodID = nullptr;

    jmethodID getDurationMethodID = nullptr;

    jmethodID seekMethodID = nullptr;

    jmethodID isPlayingMethodID = nullptr;

    jobject audioPlayer = nullptr;

};

}
