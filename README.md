# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

steps：

1. Compile skia https://skia.org/docs/user/build/ ，or unzip arm64-v8a.zip and x86-64.zip files in jniLibs.
2. Using Skia GL context.
3. Two threads: ui-thread & render-thread.
4. Only support FlexboxLayout. Measure/Layout/Draw are triggered by Vsync.
5. TouchEvents: dispatchTouchEvent/interceptTouchEvent/onTouchEvent.
6. Animations support scale/rotate/translate，the interpolator will be executed in Layout then update SkRect.
7. Supported Widgets: View/TextView/ImageView/ProgressBar/FlexboxLayout/ScrollView.
   View: rect, cornerRadius.
   ImageView: support png/gif, scaleType，cornerRadius, blur.
   TextView: SkParagraph.
   ProgressBar: circle and linear style，also can be dragged.
   SVGView: svg file.
   ShaderView: render simple fragment-shader.
   ...
8. scrollView: scroll, fling，for more optimizations.
9. RecyclerView: adapter，ViewHolder，for more optimizations.

![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example1.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example2.png)