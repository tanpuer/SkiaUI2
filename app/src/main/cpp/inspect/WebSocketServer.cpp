#include "WebSocketServer.h"
#include "native_log.h"

namespace HYSkiaUI {

WebSocketServer::WebSocketServer(std::shared_ptr<SkiaUIContext> &uiContext, int port,
                                 std::function<void(std::string)> onMessage) {
    this->uiContext = uiContext;
    this->port = port;
    this->onMessage = std::move(onMessage);

    auto jniEnv = uiContext->getJniEnv();
    auto clazz = jniEnv->FindClass("com/temple/skiaui/inspect/InspectPlugin");
    auto javaConstructor = jniEnv->GetMethodID(clazz, "<init>",
                                               "(Lcom/temple/skiaui/HYSkiaEngine;JI)V");
    runMethodId = jniEnv->GetMethodID(clazz, "run", "()V");
    sendMessageMethodId = jniEnv->GetMethodID(clazz, "sendMessage", "(Ljava/lang/String;)V");
    startListeningMethodId = jniEnv->GetMethodID(clazz, "startListening", "()V");
    waitFrontendMessageMethodId = jniEnv->GetMethodID(clazz, "waitFrontendMessage",
                                                      "()Ljava/lang/String;");
    releaseMethodId = jniEnv->GetMethodID(clazz, "release", "()V");
    printLogInChromeMethodId = jniEnv->GetMethodID(clazz, "printLogInChrome", "(Ljava/lang/String;)V");
    auto engine = uiContext->getJavaSkiaEngine();
    javaWSServer = jniEnv->NewGlobalRef(
            jniEnv->NewObject(clazz, javaConstructor, engine, reinterpret_cast<long >(this), port));
}

WebSocketServer::~WebSocketServer() {
    ALOGD("InspectServer %s", "WebSocketServer::release")
    auto jniEnv = uiContext->getJniEnv();
    jniEnv->CallVoidMethod(javaWSServer, releaseMethodId);
    jniEnv->DeleteGlobalRef(javaWSServer);
}

void WebSocketServer::run() {
    ALOGD("InspectServer %s", "WebSocketServer::run")
    auto jniEnv = uiContext->getJniEnv();
    jniEnv->CallVoidMethod(javaWSServer, runMethodId);
    printListeningMessage();
}

void WebSocketServer::sendMessage(const std::string &message) {
    ALOGD("InspectServer %s %s", "WebSocketServer::sendMessage", message.c_str())
    auto jniEnv = uiContext->getJniEnv();
    auto jString = jniEnv->NewStringUTF(message.c_str());
    jniEnv->CallVoidMethod(javaWSServer, sendMessageMethodId, jString);
}

void WebSocketServer::startListening() {
//    try {
//        ws_->accept();
//        while (true) {
//            waitFrontendMessage();
//        }
//    } catch(beast::system_error const& se) {
//        if(se.code() != websocket::error::closed)
//            std::cerr << "Error: " << se.code().message() << std::endl;
//    } catch(std::exception const& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
}

void WebSocketServer::printListeningMessage() {
    ALOGD("InspectServer Open the following link in your Chrome/Chromium browser: devtools://devtools/bundled/inspector.html?experiments=true&v8only=true&ws=0.0.0.0:8080");
}

void WebSocketServer::waitForFrontendMessageOnPause() {
    waitFrontendMessage();
}

void WebSocketServer::waitFrontendMessage() {
    auto jniEnv = uiContext->getJniEnv();
    auto message = jniEnv->CallObjectMethod(javaWSServer, waitFrontendMessageMethodId);
    if (message == nullptr) {
        return;
    }
    auto jString = static_cast<jstring>(message);
    auto result = jniEnv->GetStringUTFChars(jString, nullptr);
    std::string stringResult = result;
    if (stringResult.empty()) {
        jniEnv->ReleaseStringUTFChars(jString, result);
        return;
    }
    ALOGD("InspectServer waitFrontendMessage %s", stringResult.c_str())
    onMessage(std::move(stringResult));
    jniEnv->ReleaseStringUTFChars(jString, result);
}

void WebSocketServer::receiveMessage(const std::string &message) {
    onMessage(std::move(message));
}

void WebSocketServer::printLogInChrome(std::string &log) {
    auto jniEnv = uiContext->getJniEnv();
    jniEnv->CallVoidMethod(javaWSServer, printLogInChromeMethodId, jniEnv->NewStringUTF(log.c_str()));
}

}
