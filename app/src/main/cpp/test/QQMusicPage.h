#pragma once

#include "Page.h"
#include "AudioFFTView.h"
#include "ProgressBar.h"
#include "ImageView.h"

class QQMusicPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

    void drawOnFrame(int drawCount) override;

private:

    float progress = 0.0;

    AudioFFTView *fftView = nullptr;

    ProgressBar *progressBar = nullptr;

    ImageView *playImage = nullptr;

    ImageView *armView = nullptr;

    void updateArmView(bool play);

};
