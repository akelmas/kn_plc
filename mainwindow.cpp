#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "editor.h"
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

    auto item0 = std::make_shared<Item>(Point { 50, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    auto item1 = std::make_shared<Item>(Point { 150, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyClosed);
    auto item2 = std::make_shared<Item>(Point { 250, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    auto item3 = std::make_shared<Item>(Point { 150, 150 }, Item::Type::Coil, Item::Mode::Input, Item::State::NormallyOpen);
    auto item4 = std::make_shared<Item>(Point { 250, 250 }, Item::Type::Coil, Item::Mode::Input, Item::State::NormallyClosed);

    auto link0 = std::make_shared<Link>(item0, item1);
    auto link1 = std::make_shared<Link>(item1, item2);
    auto link2 = std::make_shared<Link>(item0, item3);
    auto link3 = std::make_shared<Link>(item1, item3);
    auto link4 = std::make_shared<Link>(item2, item4);

    Editor::instance()->items().push_back(item0);
    Editor::instance()->items().push_back(item1);
    Editor::instance()->items().push_back(item2);
    Editor::instance()->items().push_back(item3);
    Editor::instance()->items().push_back(item4);

    Editor::instance()->links().push_back(link0);
    Editor::instance()->links().push_back(link1);
    Editor::instance()->links().push_back(link2);
    Editor::instance()->links().push_back(link3);
    Editor::instance()->links().push_back(link4);

    for (auto& item : Editor::instance()->items()) {
        scene->addItem(new ItemView(item.get()));
    }

    for (auto& link : Editor::instance()->links()) {
        scene->addItem(new LinkView(link.get()));
    }

    ui->graphicsView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionContact_triggered()
{
}

void MainWindow::on_actionSelectLinePointer_triggered(bool checked)
{
    if (checked) {
        Editor::instance()->setMode(Editor::Mode::StartAddLink);
    } else {
        Editor::instance()->setMode(Editor::Mode::Idle);
    }
}
