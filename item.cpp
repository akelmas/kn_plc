#include "item.h"
#include "link.h"

std::pair<int, int> Item::pos() const
{
    return m_pos;
}

Item::Item(const std::pair<int, int>& pos, Item::Type type, Item::Mode mode, Item::State state)
    : m_pos(pos)
    , m_type(type)
    , m_mode(mode)
    , m_state(state)
{
}
