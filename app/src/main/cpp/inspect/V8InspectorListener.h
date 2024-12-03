#pragma once

#include "v8.h"

namespace HYSkiaUI {

struct V8InspectorListener {
    virtual void onConnected(const v8::Local<v8::Context> &context) = 0;
};

}
