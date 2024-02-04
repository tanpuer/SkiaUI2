# SkiaUI

switch to branch develop

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

步骤：
1. import Skia and Yoga。
   skia: https://skia.org/docs/user/build/ 
   Yoga: using directly with source code。
2. 配置好gl环境，使用Skia硬件加速。
3. 绘制模仿安卓Measure/Layout/Draw三板斧流程，只支持Flexbox布局。
4. 事件分发大体模仿安卓dispatchTouchEvent/interceptTouchEvent/onTouchEvent等，做了简化处理。
5. 动画支持scale/rotate/translate，在layout阶段更新SKMatrix，从而更新绘制的矩形。 
6. 目前只支持View/TextView/ImageView/ProgressBar/FlexboxLayout/ScrollView
   View矩形。
   ImageView支持了scaleType，圆角。
   TextView接入SkParagraph。后续想用这套试试富文本编辑器;
   ProgressBar支持圆形和直线形，可拖拽修改进度。
7. scrollView实现，滑动，fling，目前滑动效果过于僵硬，todo 优化滑动效果。
8. todo 脏区渲染
9. 实现RecyclerView，adapter，ViewHolder复用机制，目前快速滑动有问题。

![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example1.gif)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example2.gif)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example1.jpeg)
![image](https://github.com/tanpuer/SkiaUI/blob/develop/app/example2.jpeg)
