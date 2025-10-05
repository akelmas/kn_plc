#include "link.h"

Link::Link()
{
}

Item *Link::startItem() const
{
    return m_startItem;
}

Item *Link::endItem() const
{
    return m_endItem;
}

Link::Link(Item *startItem, Item *endItem) : m_startItem(startItem),
    m_endItem(endItem)
{}
