let scrollView = null;

function createRoot() {
    const { View, ScrollView, LottieView } = SkiaUI;
    scrollView = new ScrollView();
    scrollView.setWidth(1080);
    scrollView.setHeight(1080);
    scrollView.setBackgroundColor("#ffff00");

    let view = new View();
    view.setWidth(200);
    view.setHeight(200);
    view.setBackgroundColor("#ff0000");
    scrollView.addView(view);

    lottieView = new LottieView();
    lottieView.setWidth(375);
    lottieView.setHeight(240);
    lottieView.setSource("WorkspacePlanet.json");
    scrollView.addView(lottieView);

    return scrollView;
}
