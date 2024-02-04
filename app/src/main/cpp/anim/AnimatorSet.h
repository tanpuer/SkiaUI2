//
// Created by cw on 2022/6/24.
//

#ifndef SKIAUI_ANIMATORSET_H
#define SKIAUI_ANIMATORSET_H

class AnimatorSet {

public:

    AnimatorSet();

    virtual ~AnimatorSet();

private:

    bool canScale, canRotate, canTranslate;

};


#endif //SKIAUI_ANIMATORSET_H
