#include "item.h"
#include "link.h"

std::pair<int, int> Item::pos() const
{
    return m_pos;
}

Item::Type Item::type() const
{
    return m_type;
}

Item::State Item::state() const
{
    return m_state;
}

Item::Item(const std::pair<int, int>& pos, Item::Type type, Item::Mode mode, Item::State state)
    : m_pos(pos)
    , m_type(type)
    , m_mode(mode)
    , m_state(state)
{
}
