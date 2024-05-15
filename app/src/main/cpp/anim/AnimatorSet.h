#pragma once

class AnimatorSet {

public:

    AnimatorSet();

    virtual ~AnimatorSet();

private:

    bool canScale, canRotate, canTranslate;

};
