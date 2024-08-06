#include "LyricView.h"
#include "string"
#include "sstream"
#include "TextView.h"
#include "regex"

LyricView::LyricView() {
}

LyricView::~LyricView() {

}

void LyricView::setSource(const char *source) {
    const std::string input = getContext()->getAssetManager()->readFile("feng.lrc");
    std::vector<LyricLRC> result;
    std::stringstream ss(input);
    std::string token;

    std::regex timestampRegex(R"(\[(\d{2}):(\d{2})\.(\d{2})\](.*))");
    std::smatch match;
    while (std::getline(ss, token, '\n')) {
        auto lyric = LyricLRC();
        if (std::regex_search(token, match, timestampRegex)) {
            int minutes = std::stoi(match[1].str());
            int seconds = std::stoi(match[2].str());
            int milliseconds = std::stoi(match[3].str());
            std::string content = match[4];
            if (content.empty()) {
                continue;
            }
            lyric.content = std::move(content);
            lyric.timeMills = (minutes * 60 + seconds) * 1000 + milliseconds * 10;
            result.push_back(lyric);
        } else {
            ALOGD("invalid time %s", token.c_str())
        }
    }
    auto adapter = new LyricAdapter(getContext(), this);
    adapter->setData(result);
    setAdapter(adapter);
}

RecyclerViewHolder<LyricLRC> *LyricAdapter::onCreateViewHolder(int viewType) {
    auto flexLayout = new FlexboxLayout();
    flexLayout->setContext(this->context);
    flexLayout->setAlignItems(YGAlignCenter);
    flexLayout->setJustifyContent(YGJustifyCenter);
    flexLayout->setBackgroundColor(SK_ColorTRANSPARENT);
    flexLayout->setWidth(getRecyclerView()->getWidth());
    auto textView = new TextView();
    textView->setContext(this->context);
    textView->setTextSize(80);
    textView->setTextColor(SK_ColorBLACK);
    textView->setStrokeWidth(1);
    textView->setBackgroundColor(SK_ColorBLUE);
    textView->setStyle(SkPaint::kStroke_Style);
    flexLayout->addView(textView);
    auto vh = new LyricViewHolder(flexLayout);
    return vh;
}

void
LyricAdapter::onBindViewHolder(RecyclerViewHolder<LyricLRC> *viewHolder, int index, LyricLRC item) {
    viewHolder->updateView(item);
}

void LyricAdapter::onRecycleViewHolder(RecyclerViewHolder<LyricLRC> *viewHolder, LyricLRC item) {

}

void LyricViewHolder::updateView(LyricLRC item) {
    auto flexboxLayout = dynamic_cast<FlexboxLayout *>(itemView);
    auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
    textView->setMaxLines(1);
    textView->setText(item.content.c_str());
}
