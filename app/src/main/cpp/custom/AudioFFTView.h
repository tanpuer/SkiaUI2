#pragma once

#include "View.h"
#include "jni.h"

class AudioFFTView : public View {

public:

    AudioFFTView();

    ~AudioFFTView();

    void setSource(const char *path);

    void draw(SkCanvas *canvas) override;

    void onShow() override;

    void onHide() override;

    long getCurrPosition();

    long getDuration();

    void seek(long timeMills);

    bool isPlaying();

    void play();

    void pause();

private:

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

    std::unique_ptr<SkPaint> fftPaint;

    uint32_t count = 60;

    bool userPause = false;

    void innerPause();

    void innerPlay();

};
