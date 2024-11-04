# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

steps：

1. Compile skia https://skia.org/docs/user/build/ ，or **unzip arm64-v8a.zip and x86-64.zip files in jniLibs.**
2. Using Skia GL context.
3. Two threads: ui-thread & render-thread.
4. Only support FlexboxLayout. Measure/Layout/Draw are triggered by Vsync.
5. TouchEvents: dispatchTouchEvent/interceptTouchEvent/onTouchEvent.
6. Animations support scale/rotate/translate，the interpolator will be executed in Layout then update SkRect.
7. Supported Widgets: View/TextView/ImageView/ProgressBar/FlexboxLayout/ScrollView/SVGView/ShaderView.  
   View: rect, cornerRadius.  
   ImageView: support png/gif, scaleType，cornerRadius, blur.  
   TextView: use SkParagraph, use AlimamaFangYuanTiVF-Thin.ttf by default.  
   Icon: use the iconfont.woff by default.  
   ProgressBar: circle and linear style，also can be dragged.  
   SVGView: svg file.  
   ShaderView: render simple fragment-shader.  
   Lottie: render lottie.json.  
   VideoView: render local video by MediaCodec/HardwareBuffer/SkImage.  
   YUVVideoView: render video by MediaCodec/SkRuntimeEffect.  
   LyricView: parse .lrc/.srt files, use RecyclerView to render lyric.  
   Other CustomsViews: Loading, Switch, Radio...  
   ...
8. scrollView: scroll, fling，for more optimizations.
9. RecyclerView: adapter，ViewHolder，for more optimizations.
10. Page: act as the same role as Activity.
11. C++: See CppTest.cpp. 
12. js: See test.js in assets. Import v8 to evaluate javascript. 
13. kotlin: Look at JetpackComposeTest.kt in java. Import kotlin and Compose-runtime to run. 

![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example1.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example2.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example3.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example4.png)