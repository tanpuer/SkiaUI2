#pragma once

#include "functional"
#include "jni.h"

namespace HYSkiaUI {

class WebSocketServer {
public:
    WebSocketServer(int port, std::function<void(std::string)> onMessage);

    void run();

    void sendMessage(const std::string &message);

    void waitForFrontendMessageOnPause();

private:
    void startListening();

    void printListeningMessage();

    void waitFrontendMessage();

    int port;
    std::function<void(std::string)> onMessage;
    std::unique_ptr<jobject> ws = nullptr;
};

}
