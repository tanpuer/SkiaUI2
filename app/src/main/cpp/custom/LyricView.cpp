#include "LyricView.h"
#include "string"
#include "sstream"
#include "TextView.h"
#include "regex"

LyricView::LyricView() {
}

LyricView::~LyricView() {

}

void LyricView::setSourceLRC(const char *source) {
    type = LyricType::Lrc;
    const std::string input = getContext()->getAssetManager()->readFile(source);
    std::vector<Lyric> result;
    std::stringstream ss(input);
    std::string token;

    std::regex timestampRegex(R"(\[(\d{2}):(\d{2})\.(\d{2})\](.*))");
    std::smatch match;
    auto endMills = 0L;
    auto startMills = 0L;
    uint32_t lineIndex = 0;
    while (std::getline(ss, token, '\n')) {
        auto lyric = Lyric();
        if (std::regex_search(token, match, timestampRegex)) {
            int minutes = std::stoi(match[1].str());
            int seconds = std::stoi(match[2].str());
            int milliseconds = std::stoi(match[3].str());
            std::string content = match[4];
            if (content.empty()) {
                continue;
            }
            lyric.content = std::move(content);
            endMills = (minutes * 60 + seconds) * 1000 + milliseconds * 10;
            lyric.timeMills.push_back(startMills);
            lyric.timeMills.push_back(endMills);
            lyric.lineIndex = lineIndex;
            result.push_back(lyric);
            startMills = endMills;
            lineIndex++;
        } else {
            ALOGD("invalid time %s", token.c_str())
        }
    }
    auto adapter = new LyricAdapter(getContext(), this);
    adapter->setData(result);
    setAdapter(adapter);
    startTimeMills = getContext()->getCurrentTimeMills();
}

void LyricView::setSourceSRT(const char *source) {
    type = LyricType::Srt;
    const std::string input = getContext()->getAssetManager()->readFile(source);
    std::vector<Lyric> result;
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
                long totalMilliseconds = hours * 3600000 + minutes * 60000 + seconds * 1000 + milliseconds;
                lyric.timeMills.emplace_back(totalMilliseconds);
                ++rit;
            }
            std::regex_iterator<std::string::iterator> rit2(token.begin(), token.end(), patternChar);
            std::regex_iterator<std::string::iterator> rend2;
            while (rit2 != rend2) {
                auto charContent = rit2->str(1);
                lyric.contentList.emplace_back(charContent);
                lyric.content += charContent;
                ++rit2;
            }
            lyric.lineIndex = lineIndex;
            result.emplace_back(lyric);
            lineIndex++;
        }
    }
    auto adapter = new LyricAdapter(getContext(), this);
    adapter->setData(result);
    setAdapter(adapter);
    startTimeMills = getContext()->getCurrentTimeMills();
}

void LyricView::draw(SkCanvas *canvas) {
    ScrollView::draw(canvas);
    if (type == LyricType::Lrc) {
        drawLyricLRC();
    } else if (type == LyricType::Srt) {
        drawLyricSRT();
    }
}

void LyricView::drawLyricSRT() {
    auto currentTimeMills = getContext()->getCurrentTimeMills();
    auto duration = currentTimeMills - startTimeMills;
    auto start = 0L;
    auto end = 0L;
    auto index = -1;
    for (int i = adapter->startIndex; i < adapter->endIndex; ++i) {
        auto item = adapter->getItem(i);
        start = item.timeMills.front();
        end = item.timeMills.back();
        if (start <= duration && end >= duration) {
            index = i - adapter->startIndex;
            break;
        }
    }
    if (index < 0 || index >= adapter->getSize()) {
        return;
    }
    //Todo 全部markDirty
    for (int i = 0; i < children.size(); ++i) {
        auto child = children[i];
        child->markDirty();
        auto flexboxLayout = dynamic_cast<FlexboxLayout *>(child);
        auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
        auto item = adapter->getItem(i + adapter->startIndex);
        if (i == index) {
            auto jIndex = 0;
            for (int j = 0; j < item.timeMills.size(); ++j) {
                if (item.timeMills[j] > duration) {
                    jIndex = j;
                    break;
                }
            }
            auto totalLength = item.content.length();
            auto gradientLength = 0;
            for (int z = 0; z < jIndex - 1; ++z) {
                gradientLength += item.contentList[z].length();
            }
            gradientLength += (duration - item.timeMills[jIndex - 1]) * 1.0f /
                              (item.timeMills[jIndex] - item.timeMills[jIndex - 1]) *
                              item.contentList[jIndex - 1].length();
            auto percent = gradientLength * 1.0f / totalLength;
            textView->setTextGradient({SK_ColorRED, SK_ColorRED, SK_ColorBLACK, SK_ColorBLACK},
                                      {0.0, percent, percent, 1.0});
        } else {
            textView->setTextGradient({}, {});
        }
    }
}

void LyricView::drawLyricLRC() {
    auto currentTimeMills = getContext()->getCurrentTimeMills();
    auto duration = currentTimeMills - startTimeMills;
    auto start = 0L;
    auto end = 0L;
    auto index = -1;
    for (int i = adapter->startIndex; i < adapter->endIndex; ++i) {
        auto item = adapter->getItem(i);
        start = item.timeMills.front();
        end = item.timeMills.back();
        if (start <= duration && end >= duration) {
            index = i - adapter->startIndex - 1;
            break;
        }
    }
    if (index < 0 || index >= adapter->getSize()) {
        return;
    }
    //Todo 全部markDirty
    for (int i = 0; i < children.size(); ++i) {
        auto child = children[i];
        child->markDirty();
        auto flexboxLayout = dynamic_cast<FlexboxLayout *>(child);
        auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
        if (i == index) {
            auto percent = 0.0f;
            if (start == duration) {
                percent = 0.0f;
            } else if (end == duration) {
                percent = 1.0f;
            } else {
                percent = (duration - start) * 1.0f / (end - start);
            }
            textView->setTextGradient({SK_ColorRED, SK_ColorRED, SK_ColorBLACK, SK_ColorBLACK},
                                      {0.0, percent, percent, 1.0});
        } else {
            textView->setTextGradient({}, {});
        }
    }
}

RecyclerViewHolder<Lyric> *LyricAdapter::onCreateViewHolder(int viewType) {
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
    textView->setMaxLines(1);
    flexLayout->addView(textView);
    auto vh = new LyricViewHolder(flexLayout);
    return vh;
}

void
LyricAdapter::onBindViewHolder(RecyclerViewHolder<Lyric> *viewHolder, int index, Lyric item) {
    viewHolder->updateView(item);
}

void LyricAdapter::onRecycleViewHolder(RecyclerViewHolder<Lyric> *viewHolder, Lyric item) {

}

void LyricViewHolder::updateView(Lyric item) {
    auto flexboxLayout = dynamic_cast<FlexboxLayout *>(itemView);
    auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
    textView->setTextGradient({}, {});
    textView->setMaxLines(1);
    textView->setText(item.content.c_str());
}
