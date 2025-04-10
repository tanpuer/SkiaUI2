#pragma once

#include <functional>
#include <vector>
#include <list>
#include "v8-platform.h"
#include "v8.h"
#include "V8InspectorListener.h"
#include "WebSocketServer.h"
#include "V8InspectorClientImpl.h"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

class Inspector {
public:
    Inspector(std::shared_ptr<SkiaUIContext> &context, int webSocketPort);

    void startAgent();

    void printLogInChrome(std::string& log);

private:
    void onMessage(const std::string &message);

    void sendMessage(const std::string &message);

    int waitForFrontendMessage();

    v8::Handle<v8::Context> context;
    std::unique_ptr<WebSocketServer> websocketServer;
    std::unique_ptr<V8InspectorClientImpl> inspectorClient;
    std::vector<std::string> scripts = {};
    std::shared_ptr<V8Runtime> runtime;
    std::shared_ptr<SkiaUIContext> uiContext;
};

}
