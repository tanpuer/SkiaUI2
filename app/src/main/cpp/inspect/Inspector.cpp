#include "Inspector.h"
#include "native_log.h"

namespace HYSkiaUI {

Inspector::Inspector(std::shared_ptr<V8Runtime> &runtime, const int webSocketPort)
        : runtime(runtime) {
    runtime->enterContext(
            [this, webSocketPort](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                this->context = isolate->GetCurrentContext();
                websocketServer = std::make_unique<WebSocketServer>(
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
    v8_inspector::StringView protocolMessage = convertToStringView(message);
    inspectorClient->dispatchProtocolMessage(protocolMessage);

    v8::Local<v8::Object> jsonObject = parseJson(context, message);
    if (!jsonObject.IsEmpty()) {
        std::string method = getPropertyFromJson(context->GetIsolate(), jsonObject, "method");
        if (method == "Runtime.runIfWaitingForDebugger") {
            inspectorClient->schedulePauseOnNextStatement(
                    convertToStringView("For testing purpose!"));
            inspectorClient->waitFrontendMessageOnPause();
            std::for_each(listeners.begin(), listeners.end(),
                          [this](V8InspectorListener *listener) {
                              listener->onConnected(context);
                          });
        }
    }
}

void Inspector::sendMessage(const std::string &message) {
    ALOGD("Message to frontend: %s", message.c_str());
    websocketServer->sendMessage(message);
}

void Inspector::startAgent() {
    websocketServer->run();
}

int Inspector::waitForFrontendMessage() {
    websocketServer->waitForFrontendMessageOnPause();
    return 1;
}

void Inspector::addListener(V8InspectorListener *listener) {
    listeners.emplace_back(listener);
}

}
