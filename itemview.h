#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include "item.h"

#include <QGraphicsItem>

class ItemView : public QGraphicsItem {
public:
    ItemView() = delete;
    ItemView(Item* item, QGraphicsItem* parent = nullptr);
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Item* m_item;

};

#endif // ITEMVIEW_H
