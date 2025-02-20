function createFirstRoot() {
    const { View, ScrollView, LottieView, ShaderView, TextView, ImageView, VideoView, SVGView, Button,
        ProgressBar, Page, EnterExitInfo, Icon, FlexboxLayout, Switch
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

    page.backgroundColor = "#ffffff00";
    page.width = SkiaUI.innerWidth;
    page.height = SkiaUI.innerHeight;

    let scrollView = new ScrollView();
    scrollView.flex = 1;
    scrollView.flexWrap = "nowrap";
    scrollView.justifyContent = "flex-start";
    scrollView.alignItems = "center";
    scrollView.backgroundColor = "#ffffffff";
    scrollView.flexDirection = "column";

    let view = new View();
    view.width = 200;
    console.log("View width is", view.width);
    view.height = 200;
    view.backgroundColor = "#ff0000";
    view.setOnClickListener((_view) => {
        const red = Math.floor(Math.random() * 256);
        const green = Math.floor(Math.random() * 256);
        const blue = Math.floor(Math.random() * 256);
        const redHex = (red < 16 ? '0' : '') + red.toString(16);
        const greenHex = (green < 16 ? '0' : '') + green.toString(16);
        const blueHex = (blue < 16 ? '0' : '') + blue.toString(16);
        view.backgroundColor = `#${redHex}${greenHex}${blueHex}`;
    });
    scrollView.addView(view);

    let lottieView = new LottieView();
    lottieView.width = 375;
    lottieView.height = 240;
    lottieView.src = "WorkspacePlanet.json";
    let lottieFlag = true;
    lottieView.setOnClickListener((_view) => {
        if (lottieFlag) {
            lottieView.pause();
        } else {
            lottieView.start();
        }
        lottieFlag = !lottieFlag;
    });
    scrollView.addView(lottieView);

    let shaderView = new ShaderView();
    shaderView.setShaderPath("raining.glsl", ["raining.png"]);
    shaderView.width = 1080;
    shaderView.height = 520;
    scrollView.addView(shaderView);
    let flag = true;
    shaderView.setOnClickListener((_view) => {
        console.log("shaderView is clicked: ", shaderView.name, _view);
        if (flag) {
            shaderView.setShaderPath("sincos.glsl", []);
        } else {
            shaderView.setShaderPath("raining.glsl", ["raining.png"]);
        }
        flag = !flag;
    });

    let button = new Button();
    button.text = "Button";
    button.backgroundColor = "#ff0000";
    button.textSize = 60;
    button.width = 260;
    button.height = 100;
    button.setOnClickListener((view) => {
        console.log("button is clicked!");
        createSecondPage();
    });
    scrollView.addView(button);

    let imageView = new ImageView();
    imageView.width = 800;
    imageView.height = 500;
    imageView.src = "bird.gif";
    imageView.objectFill = "contain";
    imageView.backgroundColor = "#ffffff";
    let imageFlag = true;
    imageView.setOnClickListener((_view) => {
        if (imageFlag) {
            imageView.pause();
        } else {
            imageView.start();
        }
        imageFlag = !imageFlag;
    });
    scrollView.addView(imageView);

    let videoView = new VideoView();
    videoView.width = 1080;
    videoView.height = 360 * 1080 / 640;
    videoView.src = "yiluxiangbei.mp4";
    scrollView.addView(videoView);

    let svgView = new SVGView();
    svgView.src = "tiger.svg";
    svgView.width = 600;
    svgView.height = 600;
    scrollView.addView(svgView);

    let progressBar = new ProgressBar();
    progressBar.width = 800
    progressBar.height = 60;
    progressBar.backgroundColor = "#ffffff66";
    progressBar.barType = "linear";
    progressBar.barColor = "#ff0000";
    progressBar.autoMode = false;
    scrollView.addView(progressBar);

    let iconContainer = new FlexboxLayout();
    iconContainer.flexDirection = "row";
    let iconInfos = [0xe615, 0xe7ce, 0xe670, 0xe67d, 0xe606, 0xe6a2, 0xe61f];
    iconInfos.forEach((info) => {
        let icon = new Icon();
        icon.icon = info;
        icon.size = 100;
        icon.color = "#ff00ff"
        iconContainer.addView(icon);
    });
    scrollView.addView(iconContainer);

    let switchView = new Switch();
    switchView.enable = true;
    switchView.setOnChangeListener((enable) => {
        console.log("switch enable ", enable);
    });
    scrollView.addView(switchView);

    page.addView(scrollView);

    page.push(new EnterExitInfo(SkiaUI.innerWidth, 0));
    return page;
}

function createSecondPage() {
    const { View, ScrollView, LottieView, ShaderView, TextView, ImageView, VideoView, SVGView, Button,
        ProgressBar, Page, EnterExitInfo, Icon, FlexboxLayout, Switch
    } = SkiaUI;
    page = new Page();
    page.backgroundColor = "#ffffff00";
    page.width = SkiaUI.innerWidth;
    page.height = SkiaUI.innerHeight;

    let scrollView = new ScrollView();
    scrollView.flex = 1;
    scrollView.flexWrap = "nowrap";
    scrollView.justifyContent = "flex-start";
    scrollView.alignItems = "center";
    scrollView.backgroundColor = "#ffffffff";
    scrollView.flexDirection = "column";

    let view = new View();
    view.width = 200;
    console.log("View width is", view.width);
    view.height = 200;
    view.backgroundColor = "#ff0000";
    view.setOnClickListener((_view) => {
        const red = Math.floor(Math.random() * 256);
        const green = Math.floor(Math.random() * 256);
        const blue = Math.floor(Math.random() * 256);
        const redHex = (red < 16 ? '0' : '') + red.toString(16);
        const greenHex = (green < 16 ? '0' : '') + green.toString(16);
        const blueHex = (blue < 16 ? '0' : '') + blue.toString(16);
        view.backgroundColor = `#${redHex}${greenHex}${blueHex}`;
    });
    scrollView.addView(view);
    page.addView(scrollView);
    page.push(new EnterExitInfo(SkiaUI.innerWidth, 0));
}

createFirstRoot();