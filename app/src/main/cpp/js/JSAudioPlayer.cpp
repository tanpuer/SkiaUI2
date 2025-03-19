#include "JSAudioPlayer.h"

namespace HYSkiaUI {

JSAudioPlayer::JSAudioPlayer(std::shared_ptr<SkiaUIContext> &context, std::string &path) {
    this->context = context;
    auto jniEnv = context->getJniEnv();
    javaAudioPlayerClass = jniEnv->FindClass("com/temple/skiaui/audio/HYSkiaAudioTracker");
    javaAudioPlayerConstructor = jniEnv->GetMethodID(javaAudioPlayerClass, "<init>",
                                                     "(Ljava/lang/String;Lcom/temple/skiaui/HYSkiaEngine;)V");
    getFFTDataMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getFFTData", "()[F");
    auto javaSkiaEngine = context->getJavaSkiaEngine();
    audioPlayer = jniEnv->NewGlobalRef(
            jniEnv->NewObject(javaAudioPlayerClass, javaAudioPlayerConstructor,
                              jniEnv->NewStringUTF(path.c_str()), javaSkiaEngine));
    startMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "start", "()V");
    pauseMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "pause", "()V");
    currentPositionMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getCurrentPosition",
                                                  "()J");
    releaseMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "release", "()V");
    getDurationMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "getDuration", "()J");
    seekMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "seek", "(J)V");
    isPlayingMethodID = jniEnv->GetMethodID(javaAudioPlayerClass, "isPlaying", "()Z");
}

JSAudioPlayer::~JSAudioPlayer() {
    release();
}

void JSAudioPlayer::start() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, startMethodID);
}

void JSAudioPlayer::pause() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, pauseMethodID);
}

void JSAudioPlayer::release() {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, releaseMethodID);
    jniEnv->DeleteGlobalRef(audioPlayer);
    audioPlayer = nullptr;
}

long JSAudioPlayer::getCurrPosition() {
    if (audioPlayer == nullptr) {
        return 0L;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallLongMethod(audioPlayer, currentPositionMethodID);
}

void JSAudioPlayer::seek(long position) {
    if (audioPlayer == nullptr) {
        return;
    }
    auto jniEnv = context->getJniEnv();
    jniEnv->CallVoidMethod(audioPlayer, seekMethodID, position);
}

long JSAudioPlayer::getDuration() {
    if (audioPlayer == nullptr) {
        return 0L;
    }
    auto jniEnv = context->getJniEnv();
    return jniEnv->CallLongMethod(audioPlayer, getDurationMethodID);
}

}
