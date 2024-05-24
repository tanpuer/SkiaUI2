//#include <android/hardware_buffer.h>
//#include <android/hardware_buffer_jni.h>
//#include "VideoView.h"
//#include "jni.h"
//#include "gpu/ganesh/SkImageGanesh.h"
//#include "gpu/ganesh/gl/GrGLBackendSurface.h"
//#include "gpu/GrBackendSurface.h"
//#include "android/GrAHardwareBufferUtils.h"
//
//VideoView::VideoView() {
//    auto jniEnv = getContext()->getJniEnv();
//    javaVideoClass = jniEnv->FindClass("com/temple/skiaui/video/HYSkVideo");
//    javaVideoConstructor = jniEnv->GetMethodID(javaVideoClass, "<init>", "(Ljava/lang/String;)V");
//    getCurrentHardwareBuffer = jniEnv->GetMethodID(javaVideoClass, "getCurrentHardwareBuffer",
//                                                   "()Landroid/hardware/HardwareBuffer;");
//}
//
//VideoView::~VideoView() {
//
//}
//
//void VideoView::setUrl(const std::string &url) {
//    auto jniEnv = getContext()->getJniEnv();
//    javaVideo = jniEnv->NewObject(javaVideoClass, javaVideoConstructor,
//                                  jniEnv->NewStringUTF(url.c_str()));
//}
//
//void VideoView::start() {
//
//}
//
//void VideoView::pause() {
//
//}
//
//void VideoView::draw(SkCanvas *canvas) {
//    JNIEnv *env = getContext()->getJniEnv();
//    // Call the Java method
//    jobject jHardwareBuffer = env->CallObjectMethod(javaVideo, getCurrentHardwareBuffer);
//    if (jHardwareBuffer == nullptr) {
//        ALOGE("Buffer not found");
//        // Null return, handle error
//        return;
//    }
//    AHardwareBuffer *buffer = AHardwareBuffer_fromHardwareBuffer(env, jHardwareBuffer);
//    // Setup OpenGL and Skia:
//    const AHardwareBuffer *hardwareBuffer =
//            static_cast<AHardwareBuffer *>(buffer);
//    GrAHardwareBufferUtils::DeleteImageProc deleteImageProc = nullptr;
//    GrAHardwareBufferUtils::UpdateImageProc updateImageProc = nullptr;
//    GrAHardwareBufferUtils::TexImageCtx deleteImageCtx = nullptr;
//
//    AHardwareBuffer_Desc description;
//    AHardwareBuffer_describe(hardwareBuffer, &description);
//    GrBackendFormat format;
//    switch (description.format) {
//        // TODO: find out if we can detect, which graphic buffers support
//        // GR_GL_TEXTURE_2D
//        case AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGBA8, GR_GL_TEXTURE_EXTERNAL);
//        case AHARDWAREBUFFER_FORMAT_R16G16B16A16_FLOAT:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGBA16F, GR_GL_TEXTURE_EXTERNAL);
//        case AHARDWAREBUFFER_FORMAT_R5G6B5_UNORM:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGB565, GR_GL_TEXTURE_EXTERNAL);
//        case AHARDWAREBUFFER_FORMAT_R10G10B10A2_UNORM:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGB10_A2, GR_GL_TEXTURE_EXTERNAL);
//        case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGB8, GR_GL_TEXTURE_EXTERNAL);
//#if __ANDROID_API__ >= 33
//            case AHARDWAREBUFFER_FORMAT_R8_UNORM:
//    format = GrBackendFormats::MakeGL(GR_GL_R8, GR_GL_TEXTURE_EXTERNAL);
//#endif
//        default:
//            format = GrBackendFormats::MakeGL(GrGLFormat::kRGBA8, GR_GL_TEXTURE_EXTERNAL);
//    }
//
//    auto backendTex = GrAHardwareBufferUtils::MakeGLBackendTexture(
//            ThreadContextHolder::ThreadSkiaOpenGLContext.directContext.get(),
//            const_cast<AHardwareBuffer *>(hardwareBuffer), description.width,
//            description.height, &deleteImageProc, &updateImageProc, &deleteImageCtx,
//            false, format, false);
//    if (!backendTex.isValid()) {
//        ALOGE("Failed to convert HardwareBuffer to OpenGL Texture!")
//    }
//    sk_sp<SkImage> image = SkImages::BorrowTextureFrom(
//            ThreadContextHolder::ThreadSkiaOpenGLContext.directContext.get(),
//            backendTex, kTopLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
//            kOpaque_SkAlphaType, nullptr, deleteImageProc, deleteImageCtx);
//    //Todo draw skImage
//    View::draw(canvas);
//}
