#include "JSPageBinding.h"
#include "Page.h"

v8::Local<v8::FunctionTemplate>
JSPageBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto enterExitConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSPageBinding *>(data->Value());
        SkASSERT(binding);
        SkASSERT(args.Length() == 2 || args.Length() == 3);
        Page::EnterExitInfo *info = nullptr;
        if (args.Length() == 2) {
            info = new Page::EnterExitInfo(args[0]->Int32Value(), args[1]->Int32Value());
        } else if (args.Length() == 3) {
            info = new Page::EnterExitInfo(args[0]->Int32Value(), args[1]->Int32Value(),
                                           args[2]->Int32Value());
        }
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), info));
        args.GetReturnValue().Set(args.This());
    };
    auto enterExitTemplate = v8::FunctionTemplate::New(isolate, enterExitConstructor, external);
    enterExitTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    enterExitTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "EnterExitInfo"));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "EnterExitInfo"),
                enterExitTemplate->GetFunction());

    auto pageConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSPageBinding *>(data->Value());
        SkASSERT(binding);
        auto page = new Page();
        page->setContext(binding->context);
        binding->context->getPageStackManager()->push(page);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), page));
        args.GetReturnValue().Set(args.This());
    };
    auto pageTemplate = v8::FunctionTemplate::New(isolate, pageConstructor, external);
    pageTemplate->Inherit(inherit);
    pageTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    pageTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Page"));
    auto push = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsObject());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto info = static_cast<Page::EnterExitInfo *>(v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0))->Value());
        page->enterFromRight(*info);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "push",
                                           v8::FunctionTemplate::New(isolate, push));
    auto pop = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsObject());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto info = static_cast<Page::EnterExitInfo *>(v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0))->Value());
        page->exitToLeft(*info);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "pop", v8::FunctionTemplate::New(isolate, pop));
    auto onShow = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsFunction());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto callback = args[0].As<v8::Function>();
        page->showCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, callback);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "onShow",
                                           v8::FunctionTemplate::New(isolate, onShow));
    auto onHide = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsFunction());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto callback = args[0].As<v8::Function>();
        page->hideCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, callback);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "onHide",
                                           v8::FunctionTemplate::New(isolate, onHide));
    auto onCreate = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsFunction());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto callback = args[0].As<v8::Function>();
        page->createCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, callback);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "onCreate",
                                           v8::FunctionTemplate::New(isolate, onCreate));
    auto onDestroy = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsFunction());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto page = static_cast<Page *>(wrap->Value());
        auto callback = args[0].As<v8::Function>();
        page->destroyCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, callback);
    };
    pageTemplate->PrototypeTemplate()->Set(isolate, "onDestroy",
                                           v8::FunctionTemplate::New(isolate, onDestroy));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Page"), pageTemplate->GetFunction());
    return pageTemplate;
}
