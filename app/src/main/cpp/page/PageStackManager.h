//
// Created by banma-3412 on 2024/3/21.
//

#ifndef SKIAUI_PAGESTACKMANAGER_H
#define SKIAUI_PAGESTACKMANAGER_H


#include "vector"

class Page;

class PageStackManager {

public:

    PageStackManager();

    ~PageStackManager();

    void push(Page *page);

    Page *pop();

    Page *back();

    const std::vector<Page *> &getPages();

    void updateVisibility(bool isAnimationEnd);

private:

    std::vector<Page *> pages;

};


#endif //SKIAUI_PAGESTACKMANAGER_H
