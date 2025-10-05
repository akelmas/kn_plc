#include "linkview.h"

#include "item.h"

#include <QPen>

LinkView::LinkView(Link* link)
    : m_link(link)
{
    constexpr auto ctrlPointOffset { 100 };
    Point startPos = m_link->startItem()->pos();
    Point endPos = m_link->endItem()->pos();
    QPoint ctrlPoint1 = { startPos.first + ctrlPointOffset, startPos.second };
    QPoint ctrlPoint2 = { endPos.first - ctrlPointOffset, endPos.second };
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(QBrush(Qt::transparent));
    QPainterPath path(QPoint { startPos.first, startPos.second });
    path.cubicTo(ctrlPoint1, ctrlPoint2, QPoint { endPos.first, endPos.second });
    setPath(path);
}
