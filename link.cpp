#include "link.h"

Link::Link(const std::shared_ptr<Item>& startItem, const std::shared_ptr<Item>& endItem)
    : m_startItem(startItem)
    , m_endItem(endItem)
{
}

std::shared_ptr<Item> Link::startItem() const
{
    return m_startItem;
}

void Link::setStartItem(const std::shared_ptr<Item>& newStartItem)
{
    m_startItem = newStartItem;
}

std::shared_ptr<Item> Link::endItem() const
{
    return m_endItem;
}

void Link::setEndItem(const std::shared_ptr<Item>& newEndItem)
{
    m_endItem = newEndItem;
}
