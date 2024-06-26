#pragma once

#include "native_log.h"
#include "memory"
#include "SkiaUIContext.h"
#include "V8Runtime.h"


class ViewManager {

public:

    ViewManager(std::shared_ptr<SkiaUIContext>& context, std::shared_ptr<V8Runtime>& runtime);

    ~ViewManager();

    void registerHYViews();

private:

    std::shared_ptr<SkiaUIContext> context;

    std::shared_ptr<V8Runtime> runtime;

    static inline void createView(const v8::FunctionCallbackInfo<v8::Value> &args, int type);

};
