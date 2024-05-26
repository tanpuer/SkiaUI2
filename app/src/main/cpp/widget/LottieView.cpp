#include "LottieView.h"

using namespace skottie;

LottieView::LottieView() {
    paint->setColor(SK_ColorWHITE);
}

LottieView::~LottieView() {

}

void LottieView::setSource(const char *path) {
    auto assetManager = getContext()->getAssetManager();
    auto imageData = assetManager->readImage(path);
    auto length = imageData->length;
    lottieAnimation = Animation::Make(reinterpret_cast<const char *>(imageData->content), length);
    start = (getContext()->getCurrentTimeMills()) / 1000.0;
}

void LottieView::draw(SkCanvas *canvas) {
    View::draw(canvas);
    if (lottieAnimation == nullptr) {
        return;
    }
    auto current = (getContext()->getCurrentTimeMills()) / 1000.0;
    if ((current - start) > lottieAnimation->duration()) {
        start = current;
    }
    int totalFrames = lottieAnimation->duration() * lottieAnimation->fps();
    auto frame = (current - start) / lottieAnimation->duration() * totalFrames;
//    ALOGD("LottieView seek Frame: %f %f", frame, current - start)
    lottieAnimation->seekFrame(frame);
    lottieAnimation->render(canvas, &lottieRect, Animation::RenderFlag::kDisableTopLevelClipping);
}

void LottieView::layout(int l, int t, int r, int b) {
    View::layout(l, t, r, b);
    lottieRect.setLTRB(l, t, r, b);
}
