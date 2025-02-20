#include "JSFileBinding.h"
#include "File.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSFileBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto fileConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 1);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSFileBinding *>(data->Value());
        v8::String::Utf8Value utf8(args.GetIsolate(), args[0]->ToString());
        auto path = std::string(*utf8, utf8.length());
        SkASSERT(binding);
        auto file = new File(path, binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), file));
        args.GetReturnValue().Set(args.This());
    };
    auto fileTemplate = v8::FunctionTemplate::New(isolate, fileConstructor, external);
    fileTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    fileTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "File"));

    auto exist = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 0);
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetFile = static_cast<File *>(wrap->Value());
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSFileBinding *>(data->Value());
        if (targetFile != nullptr) {
            return args.GetReturnValue().Set(targetFile->exist());
        } else {
            ALOGD("View draw failed because view/canvas is nullptr");
        }
    };
    fileTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "exist"),
            v8::FunctionTemplate::New(isolate, exist, v8::External::New(isolate, this)));
    auto read = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 0);
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetFile = static_cast<File *>(wrap->Value());
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSFileBinding *>(data->Value());
        if (targetFile != nullptr) {
            return args.GetReturnValue().Set(
                    v8::String::NewFromUtf8(isolate, targetFile->read().c_str()));
        } else {
            ALOGD("View draw failed because view/canvas is nullptr");
        }
    };
    fileTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "read"),
            v8::FunctionTemplate::New(isolate, read, v8::External::New(isolate, this)));

    v8::Local<v8::Function> constructor = fileTemplate->GetFunction();
    v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
    global->Set(v8::String::NewFromUtf8(isolate, "File"), constructor);
    return fileTemplate;
}

}
