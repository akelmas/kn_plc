#ifndef ITEM_H
#define ITEM_H

#include <list>
#include <utility>

class Link;
class Item {
public:
    enum class Type : std::uint8_t {
        Contact,
        Coil,
        Any
    };

    enum class Mode : std::uint8_t {
        Input,
        Output,
        Aux
    };

    enum class State : std::uint8_t {
        NormallyOpen,
        NormallyClosed
    };

public:
    Item(const std::pair<int, int>& pos,
        Item::Type type,
        Item::Mode mode = Mode::Input,
        Item::State state = State::NormallyOpen);

    std::pair<int, int> pos() const;

    Item::Type type() const;

    Item::State state() const;

private:
    Item() = delete;
    std::pair<int,int> m_pos;
    Item::Type m_type;
    Item::Mode m_mode;
    Item::State m_state;
    std::list<std::shared_ptr<Link>> m_inputLinks;
    std::list<std::shared_ptr<Link>> m_outputLinks;
};

#endif // ITEM_H
