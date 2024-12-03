#pragma once

#include "View.h"

namespace HYSkiaUI {

template<typename T>
class RecyclerViewHolder {

public:

    RecyclerViewHolder(View *itemView) {
        assert(itemView != nullptr);
        this->itemView = itemView;
    }

    virtual ~RecyclerViewHolder() {
        delete itemView;
    }

    /**
     * 更新View
     * @param item
     */
    virtual void updateView(T item) = 0;

    virtual View *getItemView() {
        return itemView;
    }

protected:

    View *itemView;

};

}