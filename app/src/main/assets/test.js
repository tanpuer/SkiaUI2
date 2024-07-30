let page = null;

requestAnimationFrame(() => {
    if (page != null) {
        page.measure();
        page.layout(0, 0, SkiaUI.innerWidth, SkiaUI.innerHeight);
        page.draw();
    }
});

function createRoot() {
    const { View, ScrollView, LottieView, ShaderView, TextView, ImageView, VideoView, SVGView, Button,
        ProgressBar, Page, EnterExitInfo
    } = SkiaUI;
    page = new Page();
    page.onCreate(() => {
        console.log("page onCreate!");
    });
    page.onDestroy(() => {
        console.log("page onDestroy!");
    });
    page.onShow(() => {
        console.log("page onShow!");
    });
    page.onHide(() => {
        console.log("page onHide!");
    });

    page.backgroundColor = "#ffffff";
    page.width = SkiaUI.innerWidth;
    page.height = SkiaUI.innerHeight;

    let scrollView = new ScrollView();
    scrollView.flex = 1;
    scrollView.flexWrap = "nowrap";
    scrollView.justifyContent = "flex-start";
    scrollView.alignItems = "center";
    scrollView.backgroundColor = "#ffff00";
    scrollView.flexDirection = "column";

    let view = new View();
    view.width = 200;
    console.log("View width is", view.width);
    view.height = 200;
    view.backgroundColor = "#ff0000";
    scrollView.addView(view);

    let lottieView = new LottieView();
    lottieView.width = 375;
    lottieView.height = 240;
    lottieView.src = "WorkspacePlanet.json";
    scrollView.addView(lottieView);

    let shaderView = new ShaderView();
    shaderView.setShaderPath("raining.glsl", ["raining.png"]);
    shaderView.width = 1080;
    shaderView.height = 520;
    scrollView.addView(shaderView);
    let flag = true;
    shaderView.setOnClickListener((view) => {
        console.log("shaderView is clicked");
        if (flag) {
            shaderView.setShaderPath("sincos.glsl", []);
        } else {
            shaderView.setShaderPath("raining.glsl", ["raining.png"]);
        }
        flag = !flag;
    });

    let textView = new TextView();
    textView.text = "Hello World";
    textView.textSize = 100;
    scrollView.addView(textView);

    let imageView = new ImageView();
    imageView.width = 800;
    imageView.height = 500;
    imageView.src = "bird.gif";
    imageView.objectFill = "contain";
    imageView.backgroundColor = "#ffffff";
    scrollView.addView(imageView);

    let videoView = new VideoView();
    videoView.width = 1080;
    videoView.height = 360 * 1080 / 720;
    videoView.src = "BigBuckBunny.mp4";
    scrollView.addView(videoView);

    let svgView = new SVGView();
    svgView.src = "tiger.svg";
    svgView.width = 800;
    svgView.height = 800;
    scrollView.addView(svgView);

    let button = new Button();
    button.text = "Button";
    button.backgroundColor = "#ff0000";
    button.textSize = 60;
    button.width = 260;
    button.height = 100;
    button.setOnClickListener((view) => {
        console.log("button is clicked!");
    });
    scrollView.addView(button);

    let progressBar = new ProgressBar();
    progressBar.width = 800
    progressBar.height = 60;
    progressBar.backgroundColor = "#ffffff66";
    progressBar.barType = "linear";
    progressBar.barColor = "#ff0000";
    progressBar.autoMode = false;
    scrollView.addView(progressBar);

    page.addView(scrollView);

    page.push(new EnterExitInfo(SkiaUI.innerWidth, 0));
    return page;
}
