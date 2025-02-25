#include "JSSwitchBinding.h"
#include "Switch.h"
#include "color_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSSwitchBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                v8::Local<v8::FunctionTemplate> inherit,
                                v8::Local<v8::External> external) {
    auto switchConstructor = MakeJSViewConstructor<Switch, JSSwitchBinding>();
    auto switchTemplate = v8::FunctionTemplate::New(isolate, switchConstructor, external);
    switchTemplate->Inherit(inherit);
    switchTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    switchTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Switch"));

    auto enableSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsBoolean()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for Switch::setEnable; expected a bool");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<Switch *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            view->setEnabled(value->BooleanValue());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto enableGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = static_cast<Switch *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), view->getEnabled()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    switchTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "enable"),
                                                    enableGetter, enableSetter);
    auto colorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for text; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<Switch *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            v8::String::Utf8Value utf8(value);
            int r, g, b, a;
            hexToRGBA(std::string(*utf8, utf8.length()), r, g, b, a);
            view->setColor(SkColorSetARGB(a, r, g, b));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    switchTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "color"),
                                                    nullptr, colorSetter);

    auto onChange = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && (args[0]->IsFunction() || args[0]->IsNullOrUndefined()));
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<Switch *>(wrap->Value());
        auto function = v8::Local<v8::Function>::Cast(args[0]);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSSwitchBinding *>(data->Value());
        if (args[0]->IsNullOrUndefined()) {
            targetView->changeFunction.Reset();
            targetView->setOnChangeListener(nullptr);
            return;
        }
        auto newCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, function);
        targetView->changeFunction.Reset(isolate, newCallback);
        assert(binding);
        targetView->setOnChangeListener([binding, targetView](bool enable) {
            binding->runtime->enterContext(
                    [enable, targetView](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                        v8::Local<v8::Value> argv[1] = {v8::Boolean::New(isolate, enable)};
                        auto callback = targetView->changeFunction.Get(isolate);
                        if (!callback.IsEmpty()) {
                            callback->Call(isolate->GetCurrentContext(),
                                           isolate->GetCurrentContext()->Global(), 1, argv);
                        } else {
                            ALOGE("error: miss js callback for Switch");
                        }
                    });
        });
    };
    switchTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "setOnChangeListener"),
            v8::FunctionTemplate::New(isolate, onChange, v8::External::New(isolate, this)));
    v8::Local<v8::Function> constructor = switchTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Switch"), constructor);
    return switchTemplate;
}

}
