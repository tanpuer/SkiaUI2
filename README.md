# SkiaUI

Using Skia, Yoga, SurfaceView/TextureView to build a simple Flexbox-UI framework for Android.

steps：

1. Compile skia https://skia.org/docs/user/build/ ，or **unzip arm64-v8a.zip and x86-64.zip files in jniLibs.**
2. Using Skia GL context.
3. Two threads: ui-thread & render-thread.
4. Only support FlexboxLayout. Measure/Layout/Draw are triggered by Vsync.
5. TouchEvents: dispatchTouchEvent/interceptTouchEvent/onTouchEvent.
6. Animations support scale/rotate/translate，and will be performed before measure.  
   support easing functions: https://easings.net/en
7. Dirty-Render: markDirty if next draw is necessary.
8. **Supported Widgets**:   
   View, ImageView, TextView, Icon, ProgressBar, SVGView, ShaderView, Lottie, VideoView,
   YUVVideoView, ScrollView, MovingArea, RecyclerView, Swiper, Loading, Switch, Radio, Picker,
   LyricView, InputView, FlexboxLayout, Page...
9. **PlatformView**:  
   render Android-Views such as WebView/EditText by SurfaceTexture/SkImage, just like Flutter.  
   render 3D View by Filament(other engines can also supported)/SurfaceTexture/SkImage.  
   render Video(ExoPlayer)/Camera by SurfaceTexture/SkImage.  
   ...
10. **Development**:   
    |  |  |  |
    |--------|--------------------------------------|-------------------------------------------------------------------------|
    | C++    | ``                                   | See ExamplePage.cpp.
    | js     | `Import v8 to evaluate javascript`   | See test.js in assets. Support **debug in chrome**.  
    | Kotlin | `Import kotlin`<br>`Compose-runtime` | Support **Live-Edit**, save code then ui will refresh. See HYComposeExamplePage.kt in java. |                    |
    | React  | `Import react/react-reconciler`      | demo: https://github.com/tanpuer/skia-ui-react |
    | Vue    | `Import @vue/runtime-core`           | demo: https://github.com/tanpuer/skia-ui-vue. Need import CSS-Selector(TODO). |
11. **Cross Platform**  
    ios: https://github.com/tanpuer/HYSkiaUI-iOS  
12. Performance Problems:  
    Skia: Shader Compilation jank, cache GL program binaries.  

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