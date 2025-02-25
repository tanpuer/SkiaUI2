#include "JSButtonBinding.h"
#include "Button.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSButtonBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                v8::Local<v8::FunctionTemplate> inherit,
                                v8::Local<v8::External> external) {
    auto buttonConstructor = MakeJSViewConstructor<Button, JSButtonBinding>();
    auto buttonTemplate = v8::FunctionTemplate::New(isolate, buttonConstructor, external);
    buttonTemplate->Inherit(inherit);
    buttonTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    buttonTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Button"));
    v8::Local<v8::Function> constructor = buttonTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Button"), constructor);
    return buttonTemplate;
}

}
