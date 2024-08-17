#pragma once


#include "ITestDraw.h"
#include "Page.h"
#include "AudioFFTView.h"
#include "ProgressBar.h"

class QQMusicTest : public ITestDraw {

public:

    QQMusicTest() = default;

    ~QQMusicTest() = default;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    virtual View *getRootView() override;

private:

    Page *initPage(int width, int height);

    void testLyric(int drawCount, ViewGroup *root, int width, int height);

    float progress = 0.0;

    AudioFFTView *fftView = nullptr;

    ProgressBar *progressBar = nullptr;

    void calculate();

};

