#pragma once

#include <iostream>
#include <v8.h>
#include <v8-inspector.h>
#include <libplatform/libplatform.h>
#include "inspect_utils.h"
#include "V8InspctprChannelImpl.h"

class V8InspectorClientImpl final : public v8_inspector::V8InspectorClient {
public:
    V8InspectorClientImpl(std::shared_ptr<V8Runtime> &runtime,
                          const std::function<void(std::string)> &onResponse,
                          const std::function<int(void)> &onWaitFrontendMessageOnPause);

    void dispatchProtocolMessage(const v8_inspector::StringView &message_view);

    void runMessageLoopOnPause(int contextGroupId) override;

    void quitMessageLoopOnPause() override;

    void schedulePauseOnNextStatement(const v8_inspector::StringView &reason);

    void waitFrontendMessageOnPause();

private:
    v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

    static const int kContextGroupId = 1;
    std::shared_ptr<V8Runtime> runtime;
    v8::Local<v8::Context> context;
    std::unique_ptr<v8_inspector::V8Inspector> inspector;
    std::unique_ptr<v8_inspector::V8InspectorSession> session;
    std::unique_ptr<V8InspectorChannelImp> channel;
    std::function<int(void)> onWaitFrontendMessageOnPause;
    uint8_t terminated = 0;
    uint8_t run_nested_loop = 0;
};