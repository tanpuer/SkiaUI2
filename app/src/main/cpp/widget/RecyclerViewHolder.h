//
// Created by ByteDance on 2022/12/6.
//

#ifndef SKIAUI_RECYCLERVIEWHOLDER_H
#define SKIAUI_RECYCLERVIEWHOLDER_H

#include "View.h"

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

#endif //SKIAUI_RECYCLERVIEWHOLDER_H
