#include "itemview.h"

#include <QPen>

ItemView::ItemView(Item* item, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
    , m_item(item)
{
    constexpr auto rectSize { 50 };
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(QBrush(Qt::white));

    QRect rect(QPoint { m_item->pos().first - rectSize / 2, m_item->pos().second - rectSize / 2 }, QSize { rectSize, rectSize });
    setRect(rect);
}
