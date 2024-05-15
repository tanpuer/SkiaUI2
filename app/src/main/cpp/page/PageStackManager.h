#pragma once

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
