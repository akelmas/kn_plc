#ifndef LINKVIEW_H
#define LINKVIEW_H

#include "link.h"

#include <QGraphicsPathItem>

using Point = std::pair<int,int>;

class LinkView : public QGraphicsPathItem {
public:
    LinkView(Link* link);

private:
    LinkView() = default;
    Link* m_link;
};

#endif // LINKVIEW_H
