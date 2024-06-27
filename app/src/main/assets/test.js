let scrollView = null;

function createRoot() {
    const { View, ScrollView, LottieView, ShaderView, TextView, ImageView, VideoView, SVGView, Button} = SkiaUI;
    scrollView = new ScrollView();
    scrollView.width = 1080;
    scrollView.height = 1700;
    scrollView.setBackgroundColor("#ffff00");
    scrollView.setFlexDirection("column");

    let view = new View();
    view.width;
    view.width = 200;
    view.height = 200;
    view.setBackgroundColor("#ff0000");
    scrollView.addView(view);

    let lottieView = new LottieView();
    lottieView.width = 375;
    lottieView.height = 240;
    lottieView.setSource("WorkspacePlanet.json");
    scrollView.addView(lottieView);

    let shaderView = new ShaderView();
    shaderView.setShaderPath("raining.glsl", ["raining.png"]);
    shaderView.width = 1080;
    shaderView.height = 520;
    scrollView.addView(shaderView);

    let textView = new TextView();
    textView.text = "Hello World";
    textView.textSize = 100;
    scrollView.addView(textView);

    let imageView = new ImageView();
    imageView.width = 800;
    imageView.height = 500;
    imageView.src = "bird.gif";
    imageView.setBackgroundColor("#ffffff");
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
    button.setBackgroundColor("#ff0000");
    button.textSize = 30;
    button.width = 260;
    button.height = 100;
    scrollView.addView(button);

    return scrollView;
}
