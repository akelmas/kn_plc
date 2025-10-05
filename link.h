#ifndef LINK_H
#define LINK_H

#include <list>

class Item;
class Link {
public:
    Link(const std::shared_ptr<Item>& startItem, const std::shared_ptr<Item>& endItem);

    std::shared_ptr<Item> startItem() const;
    void setStartItem(const std::shared_ptr<Item>& newStartItem);
    std::shared_ptr<Item> endItem() const;
    void setEndItem(const std::shared_ptr<Item>& newEndItem);

private:
    Link() = delete;
    std::shared_ptr<Item> m_startItem;
    std::shared_ptr<Item> m_endItem;
};

#endif // LINK_H
