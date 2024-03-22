//
// Created by banma-3412 on 2024/3/21.
//

#include "PageStackManager.h"

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

const std::vector<Page *> &PageStackManager::getPages() {
    return pages;
}
