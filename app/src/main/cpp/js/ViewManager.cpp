#include "ViewManager.h"
#include "v8.h"
#include "SkiaUIContext.h"
#include "View.h"
#include "ScrollView.h"
#include "private/base/SkAssert.h"
#include "base/color_util.h"
#include "LottieView.h"

ViewManager::ViewManager(std::shared_ptr<SkiaUIContext> &context,
                         std::shared_ptr<V8Runtime> &runtime) {
    this->context = context;
    this->runtime = runtime;
}

ViewManager::~ViewManager() {

}

void ViewManager::registerHYView() {
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
        auto viewGroupTemplate = v8::FunctionTemplate::New(isolate, viewGroupConstructor, external);
        viewGroupTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        viewGroupTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ViewGroup"));
        viewGroupTemplate->Inherit(viewTemplate);
        viewGroupTemplate->PrototypeTemplate()->Set(isolate, "addView",
                                                    v8::FunctionTemplate::New(isolate, addView));
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
        default: {
            ALOGE("createView failed %d", type)
        }
    }
    SkASSERT(view);
    view->setContext(viewManager->context);
    v8::Local<v8::Object> self = args.Holder();
    self->SetInternalField(0, v8::External::New(args.GetIsolate(), view));
}
