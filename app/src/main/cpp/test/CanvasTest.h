//
// Created by cw on 2024/3/16.
//

#ifndef SKIAUI_CANVASTEST_H
#define SKIAUI_CANVASTEST_H


#include "View.h"
#include "memory"

class CanvasTest : public View {

public:

    CanvasTest();

    ~CanvasTest();

    void draw(SkCanvas *canvas) override;

    void setCircleSize(int size);

private:

    std::unique_ptr<SkPaint> paint;
    std::unique_ptr<SkPaint> pathPaint;

    int size = 0;

    long testIndex = 0;
};


#endif //SKIAUI_CANVASTEST_H
