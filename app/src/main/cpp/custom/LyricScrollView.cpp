#include "LyricScrollView.h"
#include "string"
#include "sstream"
#include "TextView.h"
#include "regex"

LyricScrollView::LyricScrollView() {

}

void LyricScrollView::setSourceSRT(const char *source) {
    const std::string input = getContext()->getAssetManager()->readFile(source);
    uint32_t lineIndex = 0;

    std::stringstream ss(input);
    std::string token;
    std::regex pattern(R"((\d{2}:\d{2}:\d{2},\d{3})(\S+?\d{2}:\d{2}:\d{2},\d{3}\S+))");
    std::smatch match;

    std::regex patternTime(R"(\d{2}:\d{2}:\d{2},\d{3})");
    std::regex patternChar(R"(\d{2}:\d{2}:\d{2},\d{3}([^0-9:]+)(?=\d{2}:\d{2}:\d{2},\d{3}|$))");
    while (std::getline(ss, token, '\n')) {
        if (std::regex_search(token, match, pattern)) {
            std::regex_iterator<std::string::iterator> rit(token.begin(), token.end(), patternTime);
            std::regex_iterator<std::string::iterator> rend;
            auto lyric = Lyric();
            while (rit != rend) {
                auto timestamp = rit->str(0);
                std::istringstream ssTime(timestamp);
                char delimiter;
                int hours, minutes, seconds, milliseconds;
                ssTime >> hours >> delimiter >> minutes >> delimiter >> seconds >> delimiter
                       >> milliseconds;
                long totalMilliseconds =
                        hours * 3600000 + minutes * 60000 + seconds * 1000 + milliseconds;
                lyric.timeMills.emplace_back(totalMilliseconds);
                ++rit;
            }
            std::regex_iterator<std::string::iterator> rit2(token.begin(), token.end(),
                                                            patternChar);
            std::regex_iterator<std::string::iterator> rend2;
            while (rit2 != rend2) {
                auto charContent = rit2->str(1);
                if (charContent == "\r") {
                    ++rit2;
                    continue;
                }
                lyric.contentList.emplace_back(charContent);
                lyric.content += charContent;
                ++rit2;
            }
            lyric.lineIndex = lineIndex;
            result.emplace_back(lyric);
            lineIndex++;
        }
    }
    for (auto i = 0; i < result.size(); i++) {
        this->addView(initItem(i));
    }
    startTimeMills = getContext()->getCurrentTimeMills();
}

void LyricScrollView::draw(SkCanvas *canvas) {
    ScrollView::draw(canvas);
    auto currentTimeMills = getContext()->getCurrentTimeMills();
    auto duration = currentTimeMills - startTimeMills;
    if (currentPositionFunc != nullptr) {
        auto positionFromPlayer = currentPositionFunc();
        ALOGD("drawLyricSRT currentPos: %ld %ld", duration, positionFromPlayer)
        duration = positionFromPlayer;
    }
    auto start = 0L;
    auto end = 0L;
    auto index = -1;
    for (int i = 0; i < result.size(); ++i) {
        auto item = result[i];
        start = item.timeMills.front();
        end = item.timeMills.back();
        if (start <= duration && end >= duration) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    if (index != currentIndex) {
        if (currentIndex >= 0 && currentIndex < children.size()) {
            auto child = children[currentIndex];
            auto flexboxLayout = dynamic_cast<FlexboxLayout *>(child);
            auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
            textView->setTextGradient({}, {});
            textView->setTextSize(60);
        }
        currentIndex = index;
        scrollToIndex(std::max(currentIndex - getDrawnCount() / 2 + 1, 0), true);
    }
    //highlight
    if (currentIndex >= 0 && currentIndex < children.size()) {
        auto child = children[currentIndex];
        auto flexboxLayout = dynamic_cast<FlexboxLayout *>(child);
        auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
        auto item = result[currentIndex];
        auto jIndex = 0;
        for (int j = 0; j < item.timeMills.size(); ++j) {
            if (item.timeMills[j] >= duration) {
                jIndex = j;
                break;
            }
        }
        auto totalLength = item.content.length();
        auto gradientLength = 0.0f;
        for (int z = 0; z < jIndex - 1; ++z) {
            gradientLength += item.contentList[z].length();
        }
        gradientLength += (duration - item.timeMills[jIndex - 1]) * 1.0f /
                          (item.timeMills[jIndex] - item.timeMills[jIndex - 1]) *
                          item.contentList[jIndex - 1].length();
        auto percent = gradientLength * 1.0f / totalLength;
        textView->setTextGradient({SK_ColorGREEN, SK_ColorGREEN, SK_ColorWHITE, SK_ColorWHITE},
                                  {0.0, percent, percent, 1.0});
        textView->setTextSize(80);
    }
}

View *LyricScrollView::initItem(int index) {
    auto flexLayout = new FlexboxLayout();
    flexLayout->setContext(this->context);
    flexLayout->setAlignItems(YGAlignCenter);
    flexLayout->setJustifyContent(YGJustifyCenter);
    flexLayout->setBackgroundColor(SK_ColorTRANSPARENT);
    flexLayout->setWidth(getWidth());
    auto textView = new TextView();
    textView->setContext(this->context);
    textView->setTextSize(60);
    textView->setTextColor(SK_ColorWHITE);
    textView->setStrokeWidth(0);
    textView->setBackgroundColor(SK_ColorTRANSPARENT);
    textView->setStyle(SkPaint::kStroke_Style);
    textView->setMaxLines(1);
    textView->setMargin({0, 40, 0, 40});
    textView->setText(result[index].content.c_str());
    flexLayout->addView(textView);
    return flexLayout;
}

void LyricScrollView::setCurrPositionFunc(std::function<long()> &&func) {
    this->currentPositionFunc = std::move(func);
}
