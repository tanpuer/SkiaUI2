#include "ViewManager.h"
#include "v8.h"
#include "SkiaUIContext.h"
#include "View.h"
#include "ScrollView.h"
#include "private/base/SkAssert.h"
#include "base/color_util.h"
#include "LottieView.h"
#include "ShaderView.h"
#include "w3c_util.h"

ViewManager::ViewManager(std::shared_ptr<SkiaUIContext> &context,
                         std::shared_ptr<V8Runtime> &runtime) {
    this->context = context;
    this->runtime = runtime;
}

ViewManager::~ViewManager() {

}

void ViewManager::registerHYViews() {
    this->runtime->enterContext([this](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
        auto external = v8::External::New(isolate, this);
        auto viewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 0);
        };
        auto setBackgroundColor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto view = static_cast<View *>(wrap->Value());
            v8::String::Utf8Value value(isolate, args[0]);
            auto hexColor = std::string(*value, value.length());
            assert(view);
            int r, g, b, a;
            hexToRGBA(hexColor, r, g, b, a);
            view->setBackgroundColor(SkColorSetARGB(a, r, g, b));
        };
        auto setWidth = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsNumber());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto view = static_cast<View *>(wrap->Value());
            auto width = args[0]->Int32Value();
            assert(view);
            view->setWidth(width);
        };
        auto setHeight = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsNumber());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto view = static_cast<View *>(wrap->Value());
            auto height = args[0]->Int32Value();
            assert(view);
            view->setHeight(height);
        };
        auto viewTemplate = v8::FunctionTemplate::New(isolate, viewConstructor, external);
        viewTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        viewTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "View"));
        viewTemplate->PrototypeTemplate()->Set(isolate, "setBackgroundColor",
                                               v8::FunctionTemplate::New(isolate,
                                                                         setBackgroundColor));
        viewTemplate->PrototypeTemplate()->Set(isolate, "setWidth",
                                               v8::FunctionTemplate::New(isolate, setWidth));
        viewTemplate->PrototypeTemplate()->Set(isolate, "setHeight",
                                               v8::FunctionTemplate::New(isolate, setHeight));
        v8::Local<v8::Function> constructor = viewTemplate->GetFunction();
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "View"), constructor);
        /**
         * ViewGroup start
         */
        auto viewGroupConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 1);
        };
        auto addView = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsObject());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto parent = static_cast<ViewGroup *>(wrap->Value());
            auto childWrap = v8::Local<v8::External>::Cast(
                    args[0]->ToObject()->GetInternalField(0));
            auto child = static_cast<View *>(childWrap->Value());
            parent->addView(child);
        };
        auto setFlexWrap = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto viewGroup = static_cast<ViewGroup *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto flexWrap = W3CToYGWrap(std::string(*utf8, utf8.length()));
            viewGroup->setFlexWrap(flexWrap);
        };
        auto setFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto viewGroup = static_cast<ViewGroup *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
            viewGroup->setFlexDirection(flexDirection);
        };
        auto viewGroupTemplate = v8::FunctionTemplate::New(isolate, viewGroupConstructor, external);
        viewGroupTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        viewGroupTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ViewGroup"));
        viewGroupTemplate->Inherit(viewTemplate);
        viewGroupTemplate->PrototypeTemplate()->Set(
                isolate, "addView",
                v8::FunctionTemplate::New(isolate, addView));
        viewGroupTemplate->PrototypeTemplate()->Set(
                isolate, "setFlexWrap",
                v8::FunctionTemplate::New(isolate, setFlexWrap));
        viewGroupTemplate->PrototypeTemplate()->Set(
                isolate, "setFlexDirection",
                v8::FunctionTemplate::New(isolate, setFlexDirection));
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "ViewGroup"),
                    viewGroupTemplate->GetFunction());
        /**
         * FlexBoxLayout start
         */
        auto flexboxConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 2);
        };
        auto flexboxTemplate = v8::FunctionTemplate::New(isolate, flexboxConstructor, external);
        flexboxTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        flexboxTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "FlexboxLayout"));
        flexboxTemplate->Inherit(viewGroupTemplate);
        auto setFlexboxFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto flexboxLayout = static_cast<FlexboxLayout *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
            flexboxLayout->setFlexDirection(flexDirection);
        };
        flexboxTemplate->InstanceTemplate()->Set(
                isolate, "setFlexDirection",
                v8::FunctionTemplate::New(isolate, setFlexboxFlexDirection));
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "FlexboxLayout"),
                    flexboxTemplate->GetFunction());
        /**
         * ScrollView start
         */
        auto scrollViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 3);
        };
        auto scrollTemplate = v8::FunctionTemplate::New(isolate, scrollViewConstructor, external);
        scrollTemplate->Inherit(flexboxTemplate);
        scrollTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        scrollTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ScrollView"));
        auto setScrollViewFlexWrap = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto scrollView = static_cast<ScrollView *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto flexWrap = W3CToYGWrap(std::string(*utf8, utf8.length()));
            scrollView->setFlexWrap(flexWrap);
        };
        scrollTemplate->InstanceTemplate()->Set(
                isolate, "setFlexWrap",
                v8::FunctionTemplate::New(isolate, setScrollViewFlexWrap));
        auto setScrollViewFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto flexboxLayout = static_cast<FlexboxLayout *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
            flexboxLayout->setFlexDirection(flexDirection);
        };
        scrollTemplate->InstanceTemplate()->Set(
                isolate, "setFlexDirection",
                v8::FunctionTemplate::New(isolate, setScrollViewFlexDirection));
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "ScrollView"), scrollTemplate->GetFunction());
        /**
         * LottieVew start
         */
        auto lottieViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 4);
        };
        auto lottieTemplate = v8::FunctionTemplate::New(isolate, lottieViewConstructor, external);
        lottieTemplate->Inherit(viewTemplate);
        lottieTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        lottieTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "LottieView"));
        auto lottieSetSource = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 1 && args[0]->IsString());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto lottieView = static_cast<LottieView *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto path = std::string(*utf8, utf8.length()).c_str();
            lottieView->setSource(path);
        };
        lottieTemplate->PrototypeTemplate()->Set(isolate, "setSource",
                                                 v8::FunctionTemplate::New(isolate,
                                                                           lottieSetSource));
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "LottieView"), lottieTemplate->GetFunction());
        /**
         * ShaderView start
         */
        auto shaderViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            ViewManager::createView(args, 5);
        };
        auto shaderTemplate = v8::FunctionTemplate::New(isolate, shaderViewConstructor, external);
        shaderTemplate->Inherit(viewTemplate);
        shaderTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        shaderTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ShaderView"));
        auto shaderSetPath = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            auto isolate = args.GetIsolate();
            assert(args.Length() == 2 && args[0]->IsString() && args[1]->IsArray());
            auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
            auto shaderView = static_cast<ShaderView *>(wrap->Value());
            v8::String::Utf8Value utf8(isolate, args[0]);
            auto path = std::string(*utf8, utf8.length());
            v8::Local<v8::Array> inputArray = v8::Local<v8::Array>::Cast(args[1]);
            uint32_t arrayLength = inputArray->Length();
            std::vector<std::string> images;
            images.reserve(arrayLength);
            for (uint32_t i = 0; i < arrayLength; ++i) {
                auto jsElement = inputArray->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
                if (jsElement->IsString()) {
                    v8::String::Utf8Value utf8String(isolate, jsElement);
                    images.emplace_back(*utf8String, utf8String.length());
                } else {
                    ALOGE("setShaderPath images must be string")
                }
            }
            shaderView->setShaderPath(path.c_str(), images);
        };
        shaderTemplate->PrototypeTemplate()->Set(isolate, "setShaderPath",
                                                 v8::FunctionTemplate::New(isolate, shaderSetPath));
        skiaUI->Set(v8::String::NewFromUtf8(isolate, "ShaderView"), shaderTemplate->GetFunction());
    });
}

void
ViewManager::createView(const v8::FunctionCallbackInfo<v8::Value> &args, int type) {
    SkASSERT(args.IsConstructCall() && args.Length() == 0);
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto viewManager = static_cast<ViewManager *>(data->Value());
    SkASSERT(viewManager);
    View *view = nullptr;
    switch (type) {
        case 0: {
            view = new View();
            break;
        }
        case 1: {
            view = new ViewGroup();
            break;
        }
        case 2: {
            view = new FlexboxLayout();
            break;
        }
        case 3: {
            view = new ScrollView();
            break;
        }
        case 4: {
            view = new LottieView();
            break;
        }
        case 5: {
            view = new ShaderView();
            break;
        }
        default: {
            ALOGE("createView failed %d", type)
        }
    }
    SkASSERT(view);
    view->setContext(viewManager->context);
    v8::Local<v8::Object> self = args.Holder();
    self->SetInternalField(0, v8::External::New(args.GetIsolate(), view));
}
