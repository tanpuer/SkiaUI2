#include "Inspector.h"
#include "native_log.h"

namespace HYSkiaUI {

Inspector::Inspector(std::shared_ptr<SkiaUIContext> &context, const int webSocketPort) {
    runtime = context->getRuntime();
    uiContext = context;
    runtime->enterContext(
            [this, webSocketPort](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                this->context = isolate->GetCurrentContext();
                websocketServer = std::make_unique<WebSocketServer>(
                        uiContext,
                        webSocketPort,
                        [this](auto &&PH1) { onMessage(std::forward<decltype(PH1)>(PH1)); }
                );
                inspectorClient = std::make_unique<V8InspectorClientImpl>(
                        this->runtime,
                        [this](auto &&PH1) { sendMessage(std::forward<decltype(PH1)>(PH1)); },
                        [this] { return waitForFrontendMessage(); }
                );
            });
}

void Inspector::onMessage(const std::string &message) {
    ALOGD("CDT message: %s", message.c_str());
    runtime->enterContext([this, message](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
        v8_inspector::StringView protocolMessage = convertToStringView(message);
        inspectorClient->dispatchProtocolMessage(protocolMessage);
    });
}

void Inspector::sendMessage(const std::string &message) {
    ALOGD("InspectServer Message to frontend: %s", message.c_str());
    websocketServer->sendMessage(message);
}

void Inspector::startAgent() {
    websocketServer->run();
}

int Inspector::waitForFrontendMessage() {
    websocketServer->waitForFrontendMessageOnPause();
    return 1;
}

void Inspector::printLogInChrome(std::string &log) {
    websocketServer->printLogInChrome(log);
}

}
