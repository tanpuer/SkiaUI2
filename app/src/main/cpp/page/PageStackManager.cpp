#include "PageStackManager.h"
#include "Page.h"

namespace HYSkiaUI {

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

void PageStackManager::removeDestroyedPage() {
    if (pages.size() > 0) {
        auto page = back();
        if (page != nullptr && page->isDestroyed()) {
            auto _page = pop();
            delete _page;
            _page = nullptr;
        }
    }
}

void PageStackManager::showCurrentPage() {
    if (pages.empty()) {
        return;
    }
    auto page = pages.back();
    page->setVisibility(true);
    page->onShow();
}

void PageStackManager::hideCurrentPage() {
    if (pages.empty()) {
        return;
    }
    auto page = pages.back();
    page->setVisibility(false);
    page->onHide();
}

void PageStackManager::hideLastPage() {
    if (pages.size() < 2) {
        return;
    }
    auto page = pages[pages.size() - 2];
    page->setVisibility(false);
    page->onHide();
}

void PageStackManager::showLastPage() {
    if (pages.size() < 2) {
        return;
    }
    auto page = pages[pages.size() - 2];
    page->setVisibility(true);
    page->onShow();
}

}
