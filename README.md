# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

steps：

1. Compile skia https://skia.org/docs/user/build/ ，or **unzip arm64-v8a.zip and x86-64.zip files in jniLibs.**
2. Using Skia GL context.
3. Two threads: ui-thread & render-thread.
4. Only support FlexboxLayout. Measure/Layout/Draw are triggered by Vsync.
5. TouchEvents: dispatchTouchEvent/interceptTouchEvent/onTouchEvent.
6. Animations support scale/rotate/translate，the interpolator will be executed in Layout then update SkRect.
7. Supported Widgets:   
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
   MovingArea: intercept TouchEvents by default and can move.  
   scrollView: scroll, fling，for more optimizations.  
   RecyclerView: adapter，ViewHolder，for more optimizations.  
   InputView: binding to Java EditText and render it's text.  
   Swiper: just like ViewPager.  
   Other CustomsViews: Loading, Switch, Radio, Picker...  
   **PlatformView**:  
                 render Android-Views such as WebView/EditText by SurfaceTexture/SkImage, just like Flutter.  
                 render CameraView by ImageReader/ByteBuffer/SkRuntimeEffect.  
                 render 3D View by Filament(other engines can also supported)/SurfaceTexture/SkImage.  
                 render Video by ExoPlayer/SurfaceTexture/SkImage.  
   ...
8. Page: act as the same role as Activity.
9. C++: See CppTest.cpp. 
10. js: See test.js in assets. Import v8 to evaluate javascript. 
11. **kotlin**: See JetpackComposeTest.kt in java. Import kotlin and Compose-runtime to run. 
12. **React**: See react_bundle.js in assets. Import react/react-reconciler to run. pls refer https://github.com/tanpuer/skia-ui-react 
13. Dirty-Render: markDirty after "draw" if necessary.

![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example1.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example2.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example3.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example4.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example5.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example6.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example7.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example8.png)