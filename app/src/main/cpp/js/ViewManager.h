#pragma once

#include "native_log.h"
#include "memory"
#include "SkiaUIContext.h"
#include "V8Runtime.h"
#include "JSViewBinding.h"
#include "JSViewGroupBinding.h"
#include "JSFlexboxLayoutBinding.h"
#include "JSScrollViewBinding.h"
#include "JSLottieViewBinding.h"
#include "JSShaderViewBinding.h"
#include "JSTextViewBinding.h"
#include "JSImageViewBinding.h"
#include "JSVideoViewBinding.h"
#include "JSSVGViewBinding.h"
#include "JSButtonBinding.h"
#include "JSProgressBarBinding.h"
#include "JSPageBinding.h"
#include "JSFileBinding.h"
#include "JSYUVVideoViewBinding.h"
#include "JSIconBinding.h"
#include "JSSwitchBinding.h"
#include "JSTextEncodingBinding.h"
#include "JSLinearAnimationBinding.h"
#include "JSAudioPlayerBinding.h"

namespace HYSkiaUI {

class ViewManager {

public:

    ViewManager(std::shared_ptr<SkiaUIContext> &context, std::shared_ptr<V8Runtime> &runtime);

    ~ViewManager();

    void registerHYViews();

private:

    std::shared_ptr<SkiaUIContext> context;

    std::shared_ptr<V8Runtime> runtime;

private:

    std::unique_ptr<JSViewBinding> jsViewBinding;
    std::unique_ptr<JSViewGroupBinding> jsViewGroupBinding;
    std::unique_ptr<JSFlexboxLayoutBinding> jsFlexboxLayoutBinding;
    std::unique_ptr<JSScrollViewBinding> jsScrollViewBinding;
    std::unique_ptr<JSLottieViewBinding> jsLottieViewBinding;
    std::unique_ptr<JSShaderViewBinding> jsShaderViewBinding;
    std::unique_ptr<JSTextViewBinding> jsTextViewBinding;
    std::unique_ptr<JSImageViewBinding> jsImageViewBinding;
    std::unique_ptr<JSVideoViewBinding> jsVideoViewBinding;
    std::unique_ptr<JSYUVVideoViewBinding> jsYUVVideoViewBinding;
    std::unique_ptr<JSSVGViewBinding> jsSVGViewBinding;
    std::unique_ptr<JSButtonBinding> jsButtonBinding;
    std::unique_ptr<JSProgressBarBinding> jsProgressBarBinding;
    std::unique_ptr<JSPageBinding> jsPageBinding;
    std::unique_ptr<JSIconBinding> jsIconBinding;
    std::unique_ptr<JSSwitchBinding> jsSwitchBinding;
    std::unique_ptr<JSLinearAnimationBinding> jsLinearAnimationBinding;

private:

    std::unique_ptr<JSFileBinding> jsFileBinding;
    std::unique_ptr<JSTextEncodingBinding> jsTextEncodingBinding;
    std::unique_ptr<JSAudioPlayerBinding> jsAudioPlayerBinding;

};

}
