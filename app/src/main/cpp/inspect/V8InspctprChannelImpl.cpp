#include "V8InspctprChannelImpl.h"
#include <functional>
#include "inspect_utils.h"

V8InspectorChannelImp::V8InspectorChannelImp(
        std::shared_ptr<V8Runtime> &runtime,
        const std::function<void(std::string)> &onResponse) {
    this->runtime = runtime;
    this->onResponse = onResponse;
}

void V8InspectorChannelImp::sendResponse(
        int callId,
        std::unique_ptr<v8_inspector::StringBuffer> message) {
    runtime->enterContext([this, &message](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
        const std::string response = convertToString(isolate, message->string());
        onResponse(response);
    });
}

void V8InspectorChannelImp::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) {
    runtime->enterContext([this, &message](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
        const std::string notification = convertToString(isolate, message->string());
        onResponse(notification);
    });
}

void V8InspectorChannelImp::flushProtocolNotifications() {
    // flush protocol notification
}
