#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "item.h"
#include "itemview.h"
#include "link.h"
#include "linkview.h"

namespace {
static std::vector<std::unique_ptr<Item>>& getItems()
{
    static std::vector<std::unique_ptr<Item>> items;
    return items;
}

static std::vector<std::unique_ptr<Link>>& getLinks()
{
    static std::vector<std::unique_ptr<Link>> links;
    return links;
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto* scene = new QGraphicsScene(0,0,500,500,ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    auto item0 = std::make_unique<Item>(Point { 50, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    auto item1 = std::make_unique<Item>(Point { 150, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyClosed);
    auto item2 = std::make_unique<Item>(Point { 250, 50 }, Item::Type::Contact, Item::Mode::Input, Item::State::NormallyOpen);
    auto item3 = std::make_unique<Item>(Point { 150, 150 }, Item::Type::Coil, Item::Mode::Input, Item::State::NormallyOpen);
    auto item4 = std::make_unique<Item>(Point { 250, 250 }, Item::Type::Coil, Item::Mode::Input, Item::State::NormallyClosed);

    auto link0 = std::make_unique<Link>(item0.get(), item1.get());
    auto link1 = std::make_unique<Link>(item1.get(), item2.get());
    auto link2 = std::make_unique<Link>(item0.get(), item3.get());
    auto link3 = std::make_unique<Link>(item1.get(), item3.get());
    auto link4 = std::make_unique<Link>(item2.get(), item4.get());

    getItems().push_back(std::move(item0));
    getItems().push_back(std::move(item1));
    getItems().push_back(std::move(item2));
    getItems().push_back(std::move(item3));
    getItems().push_back(std::move(item4));

    getLinks().push_back(std::move(link0));
    getLinks().push_back(std::move(link1));
    getLinks().push_back(std::move(link2));
    getLinks().push_back(std::move(link3));
    getLinks().push_back(std::move(link4));

    for (auto& item : getItems()) {
        scene->addItem(new ItemView(item.get()));
    }

    for (auto& link : getLinks()) {
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

