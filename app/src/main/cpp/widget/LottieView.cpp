#include "LottieView.h"

using namespace skottie;

LottieView::LottieView() {
    paint->setColor(SK_ColorWHITE);
}

LottieView::~LottieView() {

}

void LottieView::setSource(const char *path) {
    source = path;
    auto assetManager = getContext()->getAssetManager();
    auto imageData = assetManager->readImage(path);
    auto length = imageData->length;
    lottieAnimation = Animation::Make(reinterpret_cast<const char *>(imageData->content), length);
    startTime = (getContext()->getCurrentTimeMills()) / 1000.0;
}

const char* LottieView::getSource() {
    return source.c_str();
}

void LottieView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (lottieAnimation == nullptr) {
        return;
    }
    if (autoPlay) {
        auto currentTime = (getContext()->getCurrentTimeMills()) / 1000.0;
        int totalFrames = lottieAnimation->duration() * lottieAnimation->fps();
        if ((currentTime - startTime) > lottieAnimation->duration()) {
            if (repeat) {
                startTime = currentTime;
                lottieAnimation->seekFrame(0);
            } else {
                lottieAnimation->seekFrame(totalFrames);
            }
        } else {
            auto frame = (currentTime - startTime) / lottieAnimation->duration() * totalFrames;
            lottieAnimation->seekFrame(frame);
        }
    }
    lottieAnimation->render(canvas, &lottieRect, Animation::RenderFlag::kDisableTopLevelClipping);
}

void LottieView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    lottieRect.setLTRB(l, t, r, b);
}

void LottieView::start() {
    autoPlay = true;
    startTime = (getContext()->getCurrentTimeMills()) / 1000.0;
}

void LottieView::pause() {
    autoPlay = false;
}

void LottieView::setRepeat(bool repeat) {
    this->repeat = repeat;
}
