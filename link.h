#ifndef LINK_H
#define LINK_H

#include <list>

class Item;
class Link {
public:
    Link();
    Link(Item *startItem, Item *endItem);

    Item *startItem() const;

    Item *endItem() const;

private:
    Item* m_startItem;
    Item* m_endItem;

};

#endif // LINK_H
