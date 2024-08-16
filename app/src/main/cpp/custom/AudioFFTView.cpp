#include "AudioFFTView.h"

AudioFFTView::AudioFFTView() {
    fftPaint = std::make_unique<SkPaint>();
    fftPaint->setAntiAlias(true);
    fftPaint->setColor(SK_ColorRED);
    fftPaint->setStrokeWidth(5);
}

AudioFFTView::~AudioFFTView() {
    if (audioPlayer != nullptr) {
        auto jniEnv = context->getJniEnv();
        jniEnv->CallVoidMethod(audioPlayer, releaseMethodId);
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
    currentPositionMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getCurrentPosition", "()J");
    releaseMethodId = jniEnv->GetMethodID(javaAudioPlayerClass, "release", "()V");
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
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, startMethodID);
}

void AudioFFTView::onHide() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, pauseMethodID);
}

long AudioFFTView::getCurrPosition() {
    if (audioPlayer == nullptr) {
        return 0L;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallLongMethod(audioPlayer, currentPositionMethodID);
}
