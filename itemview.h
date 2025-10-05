#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include "item.h"

#include <QGraphicsRectItem>

class ItemView : public QGraphicsRectItem {
public:
    ItemView() = delete;
    ItemView(Item* item, QGraphicsItem* parent = nullptr);

private:
    Item* m_item;
};

#endif // ITEMVIEW_H
