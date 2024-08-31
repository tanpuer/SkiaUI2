#include "PageStackManager.h"
#include "Page.h"

PageStackManager::PageStackManager() {

}

PageStackManager::~PageStackManager() {

}

void PageStackManager::push(Page *page) {
    pages.emplace_back(page);
}

Page *PageStackManager::pop() {
    if (pages.empty()) {
        return nullptr;
    }
    auto page = pages.back();
    pages.pop_back();
    return page;
}

Page *PageStackManager::back() {
    if (pages.empty()) {
        return nullptr;
    }
    return pages.back();
}

const std::vector<Page *> &PageStackManager::getPages() {
    return pages;
}

void PageStackManager::updateVisibility(bool isAnimationEnd) {
    if (pages.size() > 0) {
        pages[pages.size() - 1]->setVisibility(true);
    }
    if (pages.size() < 2) {
        return;
    }
    pages[pages.size() - 2]->setVisibility(!isAnimationEnd);
}

void PageStackManager::removeDestroyedPage() {
    if (pages.size() > 0) {
        auto page = back();
        if (page != nullptr && page->isDestroyed()) {
            auto _page = pop();
            delete _page;
            _page = nullptr;
            updateVisibility(true);
        }
    }
}
