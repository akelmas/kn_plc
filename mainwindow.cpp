#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "item.h"
#include "itemview.h"
#include "link.h"
#include "linkview.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto* scene = new QGraphicsScene(0,0,500,500,ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    Item* start = new Item({ 50, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    Item* end = new Item({ 250, 250 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    Item* end2 = new Item({ 50, 350 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    Link *link  = new Link(start, end);
    Link* link2 = new Link(start, end2);

    ItemView* startItemView = new ItemView(start);
    ItemView* endItemView = new ItemView(end);
    ItemView* endItem2View = new ItemView(end2);
    LinkView* linkview = new LinkView(link);
    LinkView* linkview2 = new LinkView(link2);

    scene->addItem(linkview);
    scene->addItem(linkview2);
    scene->addItem(startItemView);
    scene->addItem(endItemView);
    scene->addItem(endItem2View);

    ui->graphicsView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionContact_triggered()
{
}

