let scrollView = null;

function createRoot() {
    const { View, ScrollView } = SkiaUI;
    scrollView = new ScrollView();
    scrollView.setWidth(1080);
    scrollView.setHeight(1080);
    let view = new View();
    view.setWidth(200);
    view.setHeight(200);
    view.setBackgroundColor("#ff0000");
    scrollView.addView(view);
    scrollView.setBackgroundColor("#ffff00");
    return scrollView;
}
