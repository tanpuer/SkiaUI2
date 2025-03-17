#pragma once

#include "functional"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

class WebSocketServer {
public:
    WebSocketServer(std::shared_ptr<SkiaUIContext> &uiContext, int port,
                    std::function<void(std::string)> onMessage);

    ~WebSocketServer();

    void run();

    void sendMessage(const std::string &message);

    void receiveMessage(const std::string &message);

    void waitForFrontendMessageOnPause();

private:
    void startListening();

    void printListeningMessage();

    void waitFrontendMessage();

    int port;
    std::function<void(std::string)> onMessage;

    std::shared_ptr<SkiaUIContext> uiContext;

    jmethodID runMethodId = nullptr;
    jmethodID sendMessageMethodId = nullptr;
    jmethodID startListeningMethodId = nullptr;
    jmethodID waitFrontendMessageMethodId = nullptr;
    jmethodID releaseMethodId = nullptr;
    jobject javaWSServer = nullptr;

};

}
