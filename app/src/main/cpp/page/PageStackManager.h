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

    void showCurrentPage();

    void hideCurrentPage();

    void hideLastPage();

    void showLastPage();

    void removeDestroyedPage();

private:

    std::vector<Page *> pages;

};
