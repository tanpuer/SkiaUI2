#include "V8InspectorClientImpl.h"

namespace HYSkiaUI {

V8InspectorClientImpl::V8InspectorClientImpl(
        std::shared_ptr<V8Runtime> &runtime,
        const std::function<void(std::string)> &onResponse,
        const std::function<int(void)> &onWaitFrontendMessageOnPause) {
    this->runtime = runtime;
    runtime->enterContext([this, onWaitFrontendMessageOnPause, onResponse](
            v8::Isolate *isolate,
            v8::Local<v8::Object> skiaUI) {
        this->onWaitFrontendMessageOnPause = onWaitFrontendMessageOnPause;
        channel.reset(new V8InspectorChannelImp(this->runtime, onResponse));
        inspector = v8_inspector::V8Inspector::create(isolate, this);
        session = inspector->connect(kContextGroupId, channel.get(), v8_inspector::StringView());
        context = isolate->GetCurrentContext();
        context->SetAlignedPointerInEmbedderData(1, this);
        v8_inspector::StringView contextName = convertToStringView("inspector");
        inspector->contextCreated(
                v8_inspector::V8ContextInfo(context, kContextGroupId, contextName));
        terminated = true;
        run_nested_loop = false;
    });
}

void V8InspectorClientImpl::dispatchProtocolMessage(const v8_inspector::StringView &message_view) {
    session->dispatchProtocolMessage(message_view);
}

void V8InspectorClientImpl::runMessageLoopOnPause(int contextGroupId) {
    if (run_nested_loop) {
        return;
    }
    terminated = false;
    run_nested_loop = true;
    runtime->enterContext([this](v8::Isolate *isolate,
                                 v8::Local<v8::Object> skiaUI) {
        while (!terminated && onWaitFrontendMessageOnPause()) {
            while (v8::platform::PumpMessageLoop(this->runtime->getPlatform(), isolate)) {}
        }
    });
    terminated = true;
    run_nested_loop = false;
}

void V8InspectorClientImpl::quitMessageLoopOnPause() {
    terminated = true;
}

v8::Local<v8::Context> V8InspectorClientImpl::ensureDefaultContextInGroup(int contextGroupId) {
    return context;
}

void V8InspectorClientImpl::schedulePauseOnNextStatement(const v8_inspector::StringView &reason) {
    session->schedulePauseOnNextStatement(reason, reason);
}

void V8InspectorClientImpl::waitFrontendMessageOnPause() {
    terminated = false;
}

}
