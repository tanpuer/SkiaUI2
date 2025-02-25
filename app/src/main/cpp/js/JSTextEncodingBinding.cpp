#include "JSTextEncodingBinding.h"
#include "TextEncoding.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSTextEncodingBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                      v8::Local<v8::FunctionTemplate> inherit,
                                      v8::Local<v8::External> external) {
    auto textEncoderConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSTextEncodingBinding *>(data->Value());
        SkASSERT(binding);
        auto textEncoder = new TextEncoding();
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), textEncoder));
        args.GetReturnValue().Set(args.This());
    };
    auto textEncoderTemplate = v8::FunctionTemplate::New(isolate, textEncoderConstructor, external);
    textEncoderTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    textEncoderTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "TextEncoder"));

    auto encode = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1);
        auto textEncoding = GetTargetView<TextEncoding>(args);
        if (textEncoding != nullptr) {
            v8::String::Utf8Value utf8(args.GetIsolate(), args[0]->ToString());
            auto value = std::string(*utf8, utf8.length());
            return args.GetReturnValue().Set(
                    v8::String::NewFromUtf8(isolate, textEncoding->encode(value)));
        } else {
            ALOGD("textEncoding is nullptr");
        }
    };
    textEncoderTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "encode"),
            v8::FunctionTemplate::New(isolate, encode, v8::External::New(isolate, this)));

    v8::Local<v8::Function> constructor = textEncoderTemplate->GetFunction();
    v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
    global->Set(v8::String::NewFromUtf8(isolate, "TextEncoder"), constructor);
    return textEncoderTemplate;
}

}