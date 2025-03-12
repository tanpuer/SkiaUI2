#include "JSViewBinding.h"
#include "View.h"
#include "color_util.h"
#include "w3c_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto viewConstructor = MakeJSViewConstructor<View, JSViewBinding>();

    auto viewTemplate = v8::FunctionTemplate::New(isolate, viewConstructor, external);
    auto viewWidthSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for width; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setWidth(value->Int32Value());
    };
    auto viewWidthGetter = [](v8::Local<v8::String> property,
                              const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getWidth()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "width"),
                                                  viewWidthGetter, viewWidthSetter);
    auto viewHeightSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for height; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setHeight(value->Int32Value());
    };
    auto viewHeightGetter = [](v8::Local<v8::String> property,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getHeight()));
    };
    viewTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    viewTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "View"));
    viewTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "height"),
                                                  viewHeightGetter, viewHeightSetter);
    auto viewBackgroundColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                        const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for backgroundColor; expected a string");
        }
        auto view = GetTargetView<View>(info);
        auto hexColor = stdString(info.GetIsolate(), value);
        view->setBackgroundColor(hexColor);
    };
    auto viewBackgroundColorGetter = [](v8::Local<v8::String> property,
                                        const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), view->getBackgroundColor()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "backgroundColor"),
            viewBackgroundColorGetter,
            viewBackgroundColorSetter);
    auto viewFlexSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for flex; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setFlex(value->Int32Value());
    };
    auto viewFlexGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getFlex()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "flex"),
            viewFlexGetter,
            viewFlexSetter);
    auto marginTopSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for marginTop; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setMarginTop(value->Int32Value());
    };
    auto marginTopGetter = [](v8::Local<v8::String> property,
                              const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getMarginTop()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "marginTop"),
            marginTopGetter,
            marginTopSetter);
    auto marginLeftSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for marginLeft; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setMarginLeft(value->Int32Value());
    };
    auto marginLeftGetter = [](v8::Local<v8::String> property,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getMarginLeft()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "marginLeft"),
            marginLeftGetter,
            marginLeftSetter);
    auto marginRightSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for marginRight; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setMarginRight(value->Int32Value());
    };
    auto marginRightGetter = [](v8::Local<v8::String> property,
                                const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getMarginRight()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "marginRight"),
            marginRightGetter,
            marginRightSetter);
    auto marginBottomSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                 const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for marginBottom; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setMarginBottom(value->Int32Value());
    };
    auto marginBottomGetter = [](v8::Local<v8::String> property,
                                 const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getMarginBottom()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "marginBottom"),
            marginBottomGetter,
            marginBottomSetter);
    auto rotateZSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for rotateZ; expected a number");
        }
        auto view = GetTargetView<View>(info);
        view->setRotateZ(value->Int32Value());
    };
    auto rotateZGetter = [](v8::Local<v8::String> property,
                            const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), view->getRotateZ()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "rotateZ"),
            rotateZGetter,
            rotateZSetter);
    auto positionSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for position; expected a string");
        }
        auto view = GetTargetView<View>(info);
        v8::String::Utf8Value utf8(info.GetIsolate(), value);
        auto position = W3CToYGPosition(std::string(*utf8, utf8.length()));
        view->setPositionType(position);
    };
    auto positionGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), YGPositionToW3C(
                view->getPositionType())));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "position"),
            positionGetter,
            positionSetter);
    auto viewNameGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto view = GetTargetView<View>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), view->name()));
    };
    viewTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "name"),
            viewNameGetter,
            nullptr
    );

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
            binding->context->getRuntime()->enterContext(
                    [view](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
                        auto external = v8::External::New(isolate, view);
                        v8::Local<v8::Value> argv[1] = {external};
                        auto callback = view->clickFunction.Get(isolate);
                        if (!callback.IsEmpty()) {
                            callback->Call(isolate->GetCurrentContext(),
                                           isolate->GetCurrentContext()->Global(), 1, argv);
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

}
