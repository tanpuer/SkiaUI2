#pragma once

#include "View.h"
#include "svg/include/SkSVGDOM.h"

class SVGView : public View {

public:

    SVGView();

    virtual ~SVGView();

    virtual void setSource(const char *path);

    const char* getSource();

    virtual void setText(const std::string& text);

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

    void setXY(int x, int y);

    const char *name() override;

private:

    sk_sp<SkSVGDOM> skSVGDom = nullptr;

    int x, y = 0;

    std::string src;

    int containerWidth, containerHeight = 0;

};
