let scrollView = null;

function createRoot() {
    const { View, ScrollView, LottieView, ShaderView} = SkiaUI;
    scrollView = new ScrollView();
    scrollView.setWidth(1080);
    scrollView.setHeight(1080);
    scrollView.setBackgroundColor("#ffff00");

    let view = new View();
    view.setWidth(200);
    view.setHeight(200);
    view.setBackgroundColor("#ff0000");
    scrollView.addView(view);

    let lottieView = new LottieView();
    lottieView.setWidth(375);
    lottieView.setHeight(240);
    lottieView.setSource("WorkspacePlanet.json");
    scrollView.addView(lottieView);

    let shaderView = new ShaderView();
    shaderView.setShaderPath("raining.glsl", ["raining.png"]);
    shaderView.setWidth(1080);
    shaderView.setHeight(520);
    scrollView.addView(shaderView);

    return scrollView;
}
