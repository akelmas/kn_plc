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

static const QPixmap& getItemImage(Item::Mode mode, Item::State state)
{
    switch (mode) {
    case Item::Mode::Input:
        return getContactImage(state);
    case Item::Mode::Output:
        return getCoilImage(state);
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

    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(boundingRect());
    painter->drawPixmap(boundingRect().toRect(), getItemImage(m_item->mode(), m_item->state()));
    constexpr auto radius { 3 };

    switch (m_item->mode()) {
    case Item::Mode::Input:
        painter->drawEllipse(QPointF(boundingRect().right(), boundingRect().center().y()), radius, radius);
    case Item::Mode::Output:
        painter->drawEllipse(QPointF(boundingRect().left(), boundingRect().center().y()), radius, radius);
        break;
    }
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
