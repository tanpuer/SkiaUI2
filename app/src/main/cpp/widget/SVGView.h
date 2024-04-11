#pragma once

#include "View.h"
#include "svg/include/SkSVGDOM.h"

class SVGView : public View {

public:

    SVGView();

    virtual ~SVGView();

    virtual void setSource(const char *path);

    virtual void setText(const std::string& text);

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

    void setXY(int x, int y);

private:

    sk_sp<SkSVGDOM> skSVGDom;

    int x, y = 0;

};
