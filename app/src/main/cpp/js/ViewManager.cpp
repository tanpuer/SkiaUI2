#include "ViewManager.h"

namespace HYSkiaUI {

ViewManager::ViewManager(std::shared_ptr<SkiaUIContext> &context,
                         std::shared_ptr<V8Runtime> &runtime) {
    this->context = context;
    this->runtime = runtime;
    jsViewBinding = std::make_unique<JSViewBinding>(context, runtime);
    jsViewGroupBinding = std::make_unique<JSViewGroupBinding>(context, runtime);
    jsFlexboxLayoutBinding = std::make_unique<JSFlexboxLayoutBinding>(context, runtime);
    jsScrollViewBinding = std::make_unique<JSScrollViewBinding>(context, runtime);
    jsLottieViewBinding = std::make_unique<JSLottieViewBinding>(context, runtime);
    jsShaderViewBinding = std::make_unique<JSShaderViewBinding>(context, runtime);
    jsTextViewBinding = std::make_unique<JSTextViewBinding>(context, runtime);
    jsImageViewBinding = std::make_unique<JSImageViewBinding>(context, runtime);
    jsVideoViewBinding = std::make_unique<JSVideoViewBinding>(context, runtime);
    jsYUVVideoViewBinding = std::make_unique<JSYUVVideoViewBinding>(context, runtime);
    jsSVGViewBinding = std::make_unique<JSSVGViewBinding>(context, runtime);
    jsButtonBinding = std::make_unique<JSButtonBinding>(context, runtime);
    jsProgressBarBinding = std::make_unique<JSProgressBarBinding>(context, runtime);
    jsPageBinding = std::make_unique<JSPageBinding>(context, runtime);
    jsIconBinding = std::make_unique<JSIconBinding>(context, runtime);
    jsSwitchBinding = std::make_unique<JSSwitchBinding>(context, runtime);
    jsLinearAnimationBinding = std::make_unique<JSLinearAnimationBinding>(context, runtime);

    jsFileBinding = std::make_unique<JSFileBinding>(context, runtime);
    jsTextEncodingBinding = std::make_unique<JSTextEncodingBinding>(context, runtime);
    jsAudioPlayerBinding = std::make_unique<JSAudioPlayerBinding>(context, runtime);
}

ViewManager::~ViewManager() {

}

void ViewManager::registerHYViews() {
    this->runtime->enterContext([this](v8::Isolate *isolate, v8::Local<v8::Object> skiaUI) {
        auto viewTemplate = jsViewBinding->registerJSView(
                isolate, skiaUI, v8::FunctionTemplate::New(isolate),
                v8::External::New(isolate, jsViewBinding.get()));
        auto viewGroupTemplate = jsViewGroupBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsViewGroupBinding.get()));
        auto flexboxTemplate = jsFlexboxLayoutBinding->registerJSView(
                isolate, skiaUI, viewGroupTemplate,
                v8::External::New(isolate, jsFlexboxLayoutBinding.get()));
        auto scrollTemplate = jsScrollViewBinding->registerJSView(
                isolate, skiaUI, flexboxTemplate,
                v8::External::New(isolate, jsScrollViewBinding.get()));
        auto lottieTemplate = jsLottieViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsLottieViewBinding.get()));
        auto shaderTemplate = jsShaderViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsShaderViewBinding.get()));
        auto textTemplate = jsTextViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsTextViewBinding.get()));
        auto imageTemplate = jsImageViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsImageViewBinding.get()));
        auto videoTemplate = jsVideoViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsVideoViewBinding.get()));
        auto yuvVideoTemplate = jsYUVVideoViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsYUVVideoViewBinding.get()));
        auto svgTemplate = jsSVGViewBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsSVGViewBinding.get()));
        auto buttonTemplate = jsButtonBinding->registerJSView(
                isolate, skiaUI, textTemplate,
                v8::External::New(isolate, jsButtonBinding.get()));
        auto progressTemplate = jsProgressBarBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsProgressBarBinding.get()));
        auto pageTemplate = jsPageBinding->registerJSView(
                isolate, skiaUI, viewGroupTemplate,
                v8::External::New(isolate, jsPageBinding.get()));
        auto iconTemplate = jsIconBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsIconBinding.get()));
        auto switchTemplate = jsSwitchBinding->registerJSView(
                isolate, skiaUI, viewTemplate,
                v8::External::New(isolate, jsSwitchBinding.get()));

        jsFileBinding->registerJSView(
                isolate, skiaUI, v8::FunctionTemplate::New(isolate),
                v8::External::New(isolate, jsFileBinding.get()));
        jsTextEncodingBinding->registerJSView(
                isolate, skiaUI, v8::FunctionTemplate::New(isolate),
                v8::External::New(isolate, jsTextEncodingBinding.get()));
        jsLinearAnimationBinding->registerJSView(
                isolate, skiaUI, v8::FunctionTemplate::New(isolate),
                v8::External::New(isolate, jsLinearAnimationBinding.get()));
        jsAudioPlayerBinding->registerJSView(
                isolate, skiaUI, v8::FunctionTemplate::New(isolate),
                v8::External::New(isolate, jsAudioPlayerBinding.get()));
    });
}

}
