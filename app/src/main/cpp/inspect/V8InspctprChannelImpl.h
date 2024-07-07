#pragma once

#include <functional>
#include <v8.h>
#include <v8-inspector.h>
#include "js/V8Runtime.h"

class V8InspectorChannelImp final : public v8_inspector::V8Inspector::Channel {
public:
    V8InspectorChannelImp(std::shared_ptr<V8Runtime> &runtime,
                          const std::function<void(std::string)> &onResponse);

    void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;

    void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

    void flushProtocolNotifications() override;

private:
    std::shared_ptr<V8Runtime> runtime;
    std::function<void(std::string)> onResponse;
};
