//#pragma once
//
//#include "View.h"
//#include "string"
//
//class VideoView : public View {
//
//public:
//
//    VideoView();
//
//    virtual ~VideoView();
//
//    void setUrl(const std::string &url);
//
//    void start();
//
//    void pause();
//
//    void draw(SkCanvas *canvas) override;
//
//private:
//
//    jclass javaVideoClass;
//
//    jmethodID javaVideoConstructor;
//
//    jmethodID getCurrentHardwareBuffer;
//
//    jobject javaVideo;
//
//};
