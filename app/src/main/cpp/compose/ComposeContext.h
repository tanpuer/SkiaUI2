#pragma once

#include "SkiaUIContext.h"
#include "unordered_map"
#include "thread"

namespace HYSkiaUI {

class ComposeContext {

public:

    static ComposeContext *getInstance() {
        static ComposeContext context;
        return &context;
    }

    std::shared_ptr<SkiaUIContext> &getUIContext() {
        return uiContextMap[std::this_thread::get_id()];
    }

    void saveContext(std::shared_ptr<SkiaUIContext> &uiContext) {
        uiContextMap[std::this_thread::get_id()] = uiContext;
    }

    void clearContext() {
        uiContextMap.erase(std::this_thread::get_id());
    }

private:

    ComposeContext() {}

    ComposeContext(ComposeContext &other) = delete;

    ComposeContext &operator=(ComposeContext &other) = delete;

    ~ComposeContext() {}

    std::unordered_map<std::__thread_id, std::shared_ptr<SkiaUIContext>> uiContextMap;

};

}
