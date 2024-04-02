//
// Created by banma-3412 on 2024/3/21.
//

#ifndef SKIAUI_PAGESTACKMANAGER_H
#define SKIAUI_PAGESTACKMANAGER_H


#include "Page.h"
#include "vector"

class PageStackManager {

public:

    static PageStackManager *getInstance() {
        static PageStackManager stackManager;
        return &stackManager;
    }

    void push(Page *page);

    Page *pop();

    Page *back();

    const std::vector<Page *> &getPages();

    void updateVisibility(bool isAnimationEnd);

private:

    std::vector<Page *> pages;

private:

    PageStackManager() = default;

    PageStackManager(PageStackManager &pageStackManager) = delete;

};


#endif //SKIAUI_PAGESTACKMANAGER_H
