#include "JSSVGViewBinding.h"
#include "SVGView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSSVGViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                 v8::Local<v8::FunctionTemplate> inherit,
                                 v8::Local<v8::External> external) {
    auto svgViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSSVGViewBinding *>(data->Value());
        SkASSERT(binding);
        auto svgView = new SVGView();
        svgView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), svgView));
        args.GetReturnValue().Set(args.This());
    };
    auto svgTemplate = v8::FunctionTemplate::New(isolate, svgViewConstructor, external);
    svgTemplate->Inherit(inherit);
    svgTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    svgTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "SVGView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for source; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto svgView = static_cast<SVGView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (svgView) {
            v8::String::Utf8Value utf8(value);
            svgView->setSource(std::string(*utf8, utf8.length()).c_str());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto svgView = static_cast<SVGView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (svgView) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), svgView->getSource()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    svgTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                 sourceGetter, sourceSetter);
    v8::Local<v8::Function> constructor = svgTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "SVGView"), constructor);
    return svgTemplate;
}

}
