#include "AudioFFTView.h"

namespace HYSkiaUI {

AudioFFTView::AudioFFTView() {
    fftPaint = std::make_unique<SkPaint>();
    fftPaint->setAntiAlias(true);
    fftPaint->setColor(SK_ColorRED);
    fftPaint->setStrokeWidth(5);
}

AudioFFTView::~AudioFFTView() {
    if (audioPlayer != nullptr) {
        auto jniEnv = context->getJniEnv();
        jniEnv->CallVoidMethod(audioPlayer, releaseMethodID);
        context->getJniEnv()->DeleteGlobalRef(audioPlayer);
    }
}

void AudioFFTView::setSource(const char *path) {
    auto jniEnv = context->getJniEnv();
//    javaAudioPlayerClass = jniEnv->FindClass("com/temple/skiaui/audio/HYSkiaAudioPlayer");
    javaAudioPlayerClass = jniEnv->FindClass("com/temple/skiaui/audio/HYSkiaAudioTracker");
    javaAudioPlayerConstructor = jniEnv->GetMethodID(javaAudioPlayerClass, "<init>",
                                                     "(Ljava/lang/String;Lcom/temple/skiaui/HYSkiaEngine;)V");
    getFFTDataMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getFFTData", "()[F");
    auto javaSkiaEngine = getContext()->getJavaSkiaEngine();
    audioPlayer = jniEnv->NewGlobalRef(
            jniEnv->NewObject(javaAudioPlayerClass, javaAudioPlayerConstructor,
                              jniEnv->NewStringUTF(path), javaSkiaEngine));
    startMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "start", "()V");
    pauseMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "pause", "()V");
    currentPositionMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getCurrentPosition",
                                                  "()J");
    releaseMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "release", "()V");
    getDurationMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getDuration", "()J");
    seekMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "seek", "(J)V");
    isPlayingMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "isPlaying", "()Z");
}

void AudioFFTView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    auto widthDiff = width * 1.0f / count;
    auto heightDiff = height * 1.0f / count;
    if (audioPlayer != nullptr) {
        auto jni = context->getJniEnv();
        auto fft = static_cast<jfloatArray>(jni->CallObjectMethod(audioPlayer, getFFTDataMethodID));
        auto length = jni->GetArrayLength(fft);
        if (length <= 0) {
            return;
        }
        jfloat *floatArray = jni->GetFloatArrayElements(fft, NULL);
        for (int i = 0; i < length; ++i) {
            auto x0 = left + i * widthDiff;
            auto y0 = bottom - floatArray[i] * 2;
            auto x1 = x0 + widthDiff;
            auto y1 = bottom;
            canvas->drawLine(x0, y0, x0, y1, *fftPaint);
        }
        jni->ReleaseFloatArrayElements(fft, floatArray, 0);
    }
}

void AudioFFTView::onShow() {
    if (userPause) {
        return;
    }
    innerPlay();
}

void AudioFFTView::onHide() {
    if (userPause) {
        return;
    }
    innerPause();
}

long AudioFFTView::getCurrPosition() {
    if (audioPlayer == nullptr) {
        return 0L;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallLongMethod(audioPlayer, currentPositionMethodID);
}

long AudioFFTView::getDuration() {
    if (audioPlayer == nullptr) {
        return 0L;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallLongMethod(audioPlayer, getDurationMethodID);
}

void AudioFFTView::seek(long timeMills) {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, seekMethodID, timeMills);
}

bool AudioFFTView::isPlaying() {
    if (audioPlayer == nullptr) {
        return false;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallBooleanMethod(audioPlayer, isPlayingMethodID);
}

void AudioFFTView::play() {
    userPause = false;
    innerPlay();
}

void AudioFFTView::pause() {
    userPause = true;
    innerPause();
}

void AudioFFTView::innerPause() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, pauseMethodID);
}

void AudioFFTView::innerPlay() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, startMethodID);
}

}