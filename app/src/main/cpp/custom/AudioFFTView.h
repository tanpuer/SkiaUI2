#pragma once

#include "View.h"
#include "jni.h"

class AudioFFTView : public View {

public:

    AudioFFTView();

    ~AudioFFTView();

    void setSource(const char *path);

    void draw(SkCanvas *canvas) override;

private:

    jclass javaAudioPlayerClass = nullptr;

    jmethodID javaAudioPlayerConstructor = nullptr;

    jmethodID getFFTDataMethodID = nullptr;

    jobject audioPlayer = nullptr;

    std::unique_ptr<SkPaint> fftPaint;

    uint32_t count = 60;

};
