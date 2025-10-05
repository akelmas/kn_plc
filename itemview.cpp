#include "itemview.h"

#include "editor.h"
#include "link.h"

#include <QPainter>
#include <QPen>
namespace{
static const QPixmap& getNoContactImage(){
    static const QPixmap pixmap = QPixmap::fromImage(QImage(":/images/NO_contact.png","PNG"));
    return pixmap;
}
static const QPixmap& getNcContactImage(){
    static const QPixmap pixmap = QPixmap::fromImage(QImage(":/images/NC_contact.png","PNG"));
    return pixmap;
}
static const QPixmap& getContactImage(Item::State state){
    switch(state){
    case Item::State::NormallyOpen:
        return getNoContactImage();
    case Item::State::NormallyClosed:
        return getNcContactImage();
    }
}
static const QPixmap& getNoCoilImage(){
    static const QPixmap pixmap = QPixmap::fromImage(QImage(":/images/coil.png","PNG"));
    return pixmap;
}
static const QPixmap& getNcCoilImage(){
    static const QPixmap pixmap = QPixmap::fromImage(QImage(":/images/negated_coil.png","PNG"));
    return pixmap;
}
static const QPixmap& getTargetItemImage()
{
    static const QPixmap pixmap = QPixmap::fromImage(QImage(":/images/target.png", "PNG"));
    return pixmap;
}

static const QPixmap& getCoilImage(Item::State state){
    switch(state){
    case Item::State::NormallyOpen:
        return getNoCoilImage();
    case Item::State::NormallyClosed:
        return getNcCoilImage();
    }
}

static const QPixmap& getItemImage(Item::Type type, Item::State state){
    switch(type){
    case Item::Type::Contact:
        return getContactImage(state);
    case Item::Type::Coil:
        return getCoilImage(state);
    case Item::Type::Any:
        return getTargetItemImage();
    }
}

}

ItemView::ItemView(Item* item, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_item(item)
{}

QRectF ItemView::boundingRect() const
{
    constexpr auto rectSize{50};
    return {QPoint { m_item->pos().first - rectSize / 2, m_item->pos().second - rectSize / 2 }, QSize { rectSize, rectSize }};
}

void ItemView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->drawPixmap(boundingRect().toRect(), getItemImage(m_item->type(), m_item->state()));
    painter->drawRect(boundingRect());

}

void ItemView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    switch (Editor::instance()->mode()) {

    case Editor::Mode::Idle:
    case Editor::Mode::StartAddLink:
        Editor::instance()->setSelectedItem(std::make_shared<Item>(m_item->pos(), Item::Type::Any));
        Editor::instance()->links().push_back(std::make_shared<Link>(std::shared_ptr<Item>(m_item), Editor::instance()->selectedItem()));
        break;
    }
}
