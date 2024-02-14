# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

步骤：

1. 编译skia。
   skia: https://skia.org/docs/user/build/ ，或者直接把jniLibs下的arm64-v8a和x86-64解压下使用。
   Yoga: 直接使用源码。
2. 配置好gl环境，使用Skia硬件加速。
3. 线程模型和Android类似，UI线程和渲染线程。
4. 绘制模仿安卓Measure/Layout/Draw三板斧流程，只支持Flexbox布局。
5. 事件分发大体模仿安卓dispatchTouchEvent/interceptTouchEvent/onTouchEvent等，做了简化处理。
6. 动画支持scale/rotate/translate，在layout阶段更新SKMatrix，从而更新绘制的矩形。
7. 目前只支持View/TextView/ImageView/ProgressBar/FlexboxLayout/ScrollView。
   View矩形，支持圆角、shader特效。
   ImageView支持了scaleType，圆角、高斯模糊。
   TextView接入SkParagraph。后续想用这套试试富文本编辑器。
   ProgressBar支持圆形和直线形，可拖拽修改进度。
8. scrollView实现，滑动，fling，目前滑动效果过于僵硬，todo 优化滑动效果。
9. 实现RecyclerView，adapter，ViewHolder复用机制，目前快速滑动有问题。

![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example1.gif)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example2.gif)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example1.jpeg)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example2.jpeg)
