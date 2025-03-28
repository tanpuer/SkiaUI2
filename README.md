# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

steps：

1. Compile skia https://skia.org/docs/user/build/ ，or **unzip arm64-v8a.zip and x86-64.zip files in jniLibs.**
2. Using Skia GL context.
3. Two threads: ui-thread & render-thread.
4. Only support FlexboxLayout. Measure/Layout/Draw are triggered by Vsync.
5. TouchEvents: dispatchTouchEvent/interceptTouchEvent/onTouchEvent.
6. Animations support scale/rotate/translate，and will be performed before measure.
7. Dirty-Render: markDirty if next draw is necessary.
8. **Supported Widgets**:   
   View, ImageView, TextView, Icon, ProgressBar, SVGView, ShaderView, Lottie, VideoView,
   YUVVideoView, ScrollView, MovingArea, RecyclerView, Swiper, Loading, Switch, Radio, Picker,
   LyricView, InputView, FlexboxLayout, Page...
9. **PlatformView**:  
   render Android-Views such as WebView/EditText by SurfaceTexture/SkImage, just like Flutter.  
   render CameraView by ImageReader/ByteBuffer/SkRuntimeEffect.  
   render 3D View by Filament(other engines can also supported)/SurfaceTexture/SkImage.  
   render Video by ExoPlayer/SurfaceTexture/SkImage.  
   ...
10. **Development**:  
    C++: See CppTest.cpp.  
    js: See test.js in assets. Import v8 to evaluate javascript.Support **debug in chrome**.  
    **kotlin**: See HYComposeExamplePage.kt in java. Import kotlin and **Compose-runtime** to run.
    Support Compose **Live-Edit**, change code and save then ui will refresh.  
    **React**: Import react/react-reconciler to run. https://github.com/tanpuer/skia-ui-react.  
    **Vue**: Import @vue/runtime-core to run. Need import CSS-Selector(TODO). https://github.com/tanpuer/skia-ui-vue.  
    ...
11. **Cross Platform**  
    ios: https://github.com/tanpuer/HYSkiaUI-iOS  
12. Performance Problems:  
    Skia: Shader Compilation jank, cache kSKSL  

![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example1.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example2.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example3.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example4.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example5.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example6.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example7.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example8.png)
![image](https://github.com/tanpuer/SkiaUI2/blob/main/app/example9.gif)