#include "JSViewBinding.h"
#include "View.h"
#include "color_util.h"

v8::Local<v8::FunctionTemplate>
JSViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto viewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSViewBinding *>(data->Value());
        SkASSERT(binding);
        auto view = new View();
        view->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), view));
        args.GetReturnValue().Set(args.This());
    };
    auto viewTemplate = v8::FunctionTemplate::New(isolate, viewConstructor, external);
    auto viewWidthSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for width; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            view->setWidth(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto viewWidthGetter = [](v8::Local<v8::String> property,
                              const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getWidth()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "width"),
                                                  viewWidthGetter, viewWidthSetter);
    auto viewHeightSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for height; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            view->setHeight(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto viewHeightGetter = [](v8::Local<v8::String> property,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getHeight()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    viewTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "View"));
    viewTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "height"),
                                                  viewHeightGetter, viewHeightSetter);
    auto viewBackgroundColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                        const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for backgroundColor; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto hexColor = std::string(*utf8, utf8.length());
            view->setBackgroundColor(hexColor);
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto viewBackgroundColorGetter = [](v8::Local<v8::String> property,
                                        const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), view->getBackgroundColor()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "backgroundColor"),
            viewBackgroundColorGetter,
            viewBackgroundColorSetter);
    auto viewFlexSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for flex; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            view->setFlex(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto viewFlexGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (view) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getFlex()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "flex"),
            viewFlexGetter,
            viewFlexSetter);

    auto onClick = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && (args[0]->IsFunction() || args[0]->IsNullOrUndefined()));
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<View *>(wrap->Value());
        auto function = v8::Local<v8::Function>::Cast(args[0]);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSViewBinding *>(data->Value());
        if (args[0]->IsNullOrUndefined()) {
            targetView->clickFunction.Reset();
            targetView->setOnClickListener(nullptr);
            return;
        }
        auto newCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, function);
        targetView->clickFunction.Reset(isolate, newCallback);
        assert(binding);
        targetView->setOnClickListener([binding](View *view) {
            binding->runtime->enterContext(
                    [binding, view](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                        auto external = v8::External::New(isolate, view);
                        v8::Local<v8::Value> argv[1] = {external};
                        auto callback = view->clickFunction.Get(isolate);
                        if (!callback.IsEmpty()) {
                            callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
                        } else {
                            ALOGE("error: miss js callback for View");
                        }
                    });
        });
    };
    viewTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "setOnClickListener"),
            v8::FunctionTemplate::New(isolate, onClick, v8::External::New(isolate, this)));
    auto measure = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 0);
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<View *>(wrap->Value());
        if (targetView != nullptr) {
            targetView->measure();
        }
    };
    viewTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "measure"),
            v8::FunctionTemplate::New(isolate, measure, v8::External::New(isolate, this)));
    auto layout = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 4 && args[0]->IsNumber() && args[1]->IsNumber() &&
               args[2]->IsNumber() && args[3]->IsNumber());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<View *>(wrap->Value());
        if (targetView != nullptr) {
            targetView->layout(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(),
                               args[3]->Int32Value());
        }
    };
    viewTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "layout"),
            v8::FunctionTemplate::New(isolate, layout, v8::External::New(isolate, this)));
    auto draw = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 0);
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<View *>(wrap->Value());
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSViewBinding *>(data->Value());
        auto canvas = binding->context->getCanvas();
        if (targetView != nullptr && canvas != nullptr) {
            targetView->draw(canvas);
        } else {
            ALOGD("View draw failed because view/canvas is nullptr");
        }
    };
    viewTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "draw"),
            v8::FunctionTemplate::New(isolate, draw, v8::External::New(isolate, this)));
    v8::Local<v8::Function> constructor = viewTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "View"), constructor);
    return viewTemplate;
}
