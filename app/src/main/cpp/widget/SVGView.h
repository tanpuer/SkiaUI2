//
// Created by banma-3412 on 2024/3/14.
//

#ifndef SKIAUI_SVGVIEW_H
#define SKIAUI_SVGVIEW_H


#include "View.h"
#include "svg/include/SkSVGDOM.h"

class SVGView : public View {

public:

    SVGView();

    virtual ~SVGView();

    virtual void setSource(const char *path);

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

private:

    sk_sp<SkSVGDOM> skSVGDom;

};


#endif //SKIAUI_SVGVIEW_H
