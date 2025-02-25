#include "JSSVGViewBinding.h"
#include "SVGView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSSVGViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                 v8::Local<v8::FunctionTemplate> inherit,
                                 v8::Local<v8::External> external) {
    auto svgViewConstructor = MakeJSViewConstructor<SVGView, JSSVGViewBinding>();
    auto svgTemplate = v8::FunctionTemplate::New(isolate, svgViewConstructor, external);
    svgTemplate->Inherit(inherit);
    svgTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    svgTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "SVGView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for source; expected a string");
        }
        auto svgView = GetTargetView<SVGView>(info);
        svgView->setSource(stdString(info.GetIsolate(), value).c_str());
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto svgView = GetTargetView<SVGView>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), svgView->getSource()));
    };
    svgTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                 sourceGetter, sourceSetter);
    v8::Local<v8::Function> constructor = svgTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "SVGView"), constructor);
    return svgTemplate;
}

}
