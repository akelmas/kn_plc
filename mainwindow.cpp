/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "mainwindow.h"
#include "settingsform.h"

#include <QtWidgets>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QProcess>


const int InsertTextButton = 100;

//! [0]
MainWindow::MainWindow()
{
    createActions();
    createToolBox();
    createMenus();


    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 485, 6400));
    scene->setBackgroundBrush(QPixmap(":/images/scene_background.svg"));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();
    settingsForm=new SettingsForm;


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("KN Ladder Editor"));
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [0]


//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    int modeId;
    if(id<12){
        modeId=0;
    }else if(id<28){
        modeId=1;
    }else{
        modeId=2;
    }
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id),DiagramItem::MODE(modeId));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]

//! [3]
void MainWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            arrow->startItem()->arrows_out.removeOne(arrow);
            arrow->endItem()->arrows_in.removeOne(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == DiagramItem::Type)
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        scene->removeItem(item);
        delete item;
    }
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::makeNO()
{
    if (scene->selectedItems().isEmpty())
        return;

    DiagramItem *selectedItem = qgraphicsitem_cast<DiagramItem *>(scene->selectedItems().first());
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();
    if((selectedItem)->diagramType()<8){
        if(!(selectedItem->normallyOpen)){
            selectedItem->normallyOpen=true;
            selectedItem->myPath=selectedItem->no_contact;
        }
    }
    else{
        if(!(selectedItem->normallyOpen)){
            selectedItem->normallyOpen=true;
            selectedItem->myPath=selectedItem->no_coil;
        }

    }

    selectedItem->myPath.addText(QPointF(-32,30),QFont(),selectedItem->title);
    selectedItem->setPath(selectedItem->myPath);


}
//! [5]

//! [6]
void MainWindow::makeNC()
{
    if (scene->selectedItems().isEmpty())
        return;

    DiagramItem *selectedItem = qgraphicsitem_cast<DiagramItem *>(scene->selectedItems().first());
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();
    if((selectedItem)->diagramType()<8){
        if((selectedItem->normallyOpen)){
            selectedItem->normallyOpen=false;
            selectedItem->myPath=selectedItem->nc_contact;
        }
    }
    else{
        if((selectedItem->normallyOpen)){
            selectedItem->normallyOpen=false;
            selectedItem->myPath=selectedItem->nc_coil;
            selectedItem->setPath(selectedItem->nc_coil);
        }

    }
    selectedItem->myPath.addText(QPointF(-32,30),QFont(),selectedItem->title);

    selectedItem->setPath(selectedItem->myPath);

}

void MainWindow::refreshPortList()
{
    portList->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        portList->addItem(info.systemLocation());
    }

}

void MainWindow::flashProgram()
{
    QFile file(":/firmware/KN_firmware.cpp");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString data="";
    data.append("#include <Arduino.h>\n");
    data.append(settingsForm->settingsResult);
    while (!in.atEnd()) {
        QString line = in.readLine();
        data.append(line);
        data.append('\n');

    }
    data.append("void _process(){\n");
    data.append(scene->generateBooleanExpressions());
    data.append("}");

    QString path;
    QString appData;
    appData.append(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    appData.append("/KN");

    QDir dir(appData+"/src");
    dir.removeRecursively();
    if (!dir.exists())
        qWarning("Cannot find the example directory. Creating");
    if(dir.mkpath(appData))
        qWarning("Already exits");

    if(dir.mkpath(appData+"/src"))
        qWarning("dir created: out");
    path=appData+"/src/out.cpp";
    QFile ofile(path);
    if (!ofile.open(QIODevice::ReadWrite|QIODevice::Text)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QProcess compiler;
    QStringList arguments;
//    arguments.clear();
//    arguments.append("run");
//    arguments.append("--project-dir");
//    arguments.append(appData);
//    arguments.append("-t");
//    arguments.append("clean");
//    qInfo()<<compiler.execute("/usr/local/bin/platformio",arguments);

    QTextStream out(&ofile);
    out << data.toUtf8();

    arguments.clear();
    arguments.append("run");
    arguments.append("--project-dir");
    arguments.append(appData);
    arguments.append("-t");
    arguments.append("upload");
    qInfo()<<compiler.startDetached("/usr/local/bin/platformio",arguments);


    compiler.terminate();



}

void MainWindow::fw_groupClicked(int id)
{
    switch (id) {
    case 0:
        qInfo()<<"settings"<<endl;
        openSettings();
        break;
    case 1:
        qInfo()<<"refresh"<<endl;
        refreshPortList();
        break;
    case 2:
        flashProgram();
        qInfo()<<"upload"<<endl;
        break;

    }

}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);

}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
            qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About KN Ladder Editor"),
                       tr("<b>KN Ladder Editor</b> helps programming KN PLC"));
}
//! [20]

//! [21]
void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    //DIN
    layout->addWidget(createCellWidget(tr("DigitalIn0"), DiagramItem::DigitalIn0,DiagramItem::INPUTONLY), 0, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn1"), DiagramItem::DigitalIn1,DiagramItem::INPUTONLY), 1, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn2"), DiagramItem::DigitalIn2,DiagramItem::INPUTONLY), 2, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn3"), DiagramItem::DigitalIn3,DiagramItem::INPUTONLY), 3, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn4"), DiagramItem::DigitalIn4,DiagramItem::INPUTONLY), 4, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn5"), DiagramItem::DigitalIn5,DiagramItem::INPUTONLY), 5, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn6"), DiagramItem::DigitalIn6,DiagramItem::INPUTONLY), 6, 0);
    layout->addWidget(createCellWidget(tr("DigitalIn7"), DiagramItem::DigitalIn7,DiagramItem::INPUTONLY), 7, 0);
    layout->addWidget(createCellWidget(tr("TIM1"), DiagramItem::TIM1,DiagramItem::INPUTONLY), 9, 0);
    layout->addWidget(createCellWidget(tr("CNT1"), DiagramItem::CNT1,DiagramItem::INPUTONLY), 10, 0);
    layout->addWidget(createCellWidget(tr("CNT2"), DiagramItem::CNT2,DiagramItem::INPUTONLY), 11, 0);
    //AUX

    layout->addWidget(createCellWidget(tr("AUX0"), DiagramItem::AUX0,DiagramItem::BOTH), 0, 1);
    layout->addWidget(createCellWidget(tr("AUX1"), DiagramItem::AUX1,DiagramItem::BOTH), 1, 1);
    layout->addWidget(createCellWidget(tr("AUX2"), DiagramItem::AUX2,DiagramItem::BOTH), 2, 1);
    layout->addWidget(createCellWidget(tr("AUX3"), DiagramItem::AUX3,DiagramItem::BOTH), 3, 1);
    layout->addWidget(createCellWidget(tr("AUX4"), DiagramItem::AUX4,DiagramItem::BOTH), 4, 1);
    layout->addWidget(createCellWidget(tr("AUX5"), DiagramItem::AUX5,DiagramItem::BOTH), 5, 1);
    layout->addWidget(createCellWidget(tr("AUX6"), DiagramItem::AUX6,DiagramItem::BOTH), 6, 1);
    layout->addWidget(createCellWidget(tr("AUX7"), DiagramItem::AUX7,DiagramItem::BOTH), 7, 1);
    layout->addWidget(createCellWidget(tr("TIM1EN"), DiagramItem::TIM1EN,DiagramItem::OUTPUTONLY), 9, 1);
    layout->addWidget(createCellWidget(tr("CNT1SET"), DiagramItem::CNT1SET,DiagramItem::OUTPUTONLY), 10, 1);
    layout->addWidget(createCellWidget(tr("CNT2SET"), DiagramItem::CNT2SET,DiagramItem::OUTPUTONLY), 11, 1);
    //DOUT

    layout->addWidget(createCellWidget(tr("DigitalOut0"), DiagramItem::DigitalOut0,DiagramItem::BOTH), 0, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut1"), DiagramItem::DigitalOut1,DiagramItem::BOTH), 1, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut2"), DiagramItem::DigitalOut2,DiagramItem::BOTH), 2, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut3"), DiagramItem::DigitalOut3,DiagramItem::BOTH), 3, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut4"), DiagramItem::DigitalOut4,DiagramItem::BOTH), 4, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut5"), DiagramItem::DigitalOut5,DiagramItem::BOTH), 5, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut6"), DiagramItem::DigitalOut6,DiagramItem::BOTH), 6, 2);
    layout->addWidget(createCellWidget(tr("DigitalOut7"), DiagramItem::DigitalOut7,DiagramItem::BOTH), 7, 2);
    layout->addWidget(createCellWidget(tr("TIM1STOP"), DiagramItem::TIM1STOP,DiagramItem::OUTPUTONLY), 9, 2);
    layout->addWidget(createCellWidget(tr("CNT1RESET"), DiagramItem::CNT1RESET,DiagramItem::OUTPUTONLY), 10, 2);
    layout->addWidget(createCellWidget(tr("CNT2RESET"), DiagramItem::CNT2RESET,DiagramItem::OUTPUTONLY), 11, 2);

    //! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 12, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);


    //! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Input and Output blocks"));
}
//! [22]

//! [23]
void MainWindow::createActions()
{
    //! [23]

    NO_action=new QAction(QIcon(":/images/NO_contact.png"),tr("Normally Open"),this);
    NO_action->setShortcut(tr("Ctrl+O"));
    NO_action->setStatusTip(tr("Click to make it normally open"));
    connect(NO_action,SIGNAL(triggered()),this,SLOT(makeNO()));

    NC_action=new QAction(QIcon(":/images/NC_contact.png"),tr("Normally Closed"),this);
    NC_action->setShortcut(tr("Ctrl+C"));
    NC_action->setStatusTip(tr("Click to make it normally closed"));
    connect(NC_action,SIGNAL(triggered()),this,SLOT(makeNC()));



    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));


}

//! [24]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(NO_action);
    itemMenu->addAction(NC_action);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
    //! [25]
    //!
    //!
    //!


    portList = new QComboBox();
    refreshPortList();
    QToolButton *btn_flash=new QToolButton;
    btn_flash->setText(tr("Upload"));
    QToolButton *btn_refresh=new QToolButton;
    btn_refresh->setText(tr("Refresh"));
    QToolButton *btn_settings=new QToolButton;
    btn_settings->setText(tr("Settings"));

    QButtonGroup *fw_group=new QButtonGroup(this);

    fw_group->addButton(btn_settings,0);
    fw_group->addButton(btn_refresh,1);
    fw_group->addButton(btn_flash,2);
    connect(fw_group,SIGNAL(buttonClicked(int)),this,SLOT(fw_groupClicked(int)));
    flashToolbar=addToolBar(tr("Flashing"));
    flashToolbar->addWidget(btn_settings);
    flashToolbar->addWidget(portList);
    flashToolbar->addWidget(btn_refresh);
    flashToolbar->addWidget(btn_flash);
    flashToolbar->setMovable(false);
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
    pointerToolbar->setMovable(false);

    addToolBarBreak();

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(NO_action);
    editToolBar->addAction(NC_action);
    editToolBar->setMovable(false);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

    //! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
    //! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->setMovable(false);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);
    colorToolBar->setMovable(false);








    //! [27]
}

void MainWindow::openSettings()
{

    settingsForm->show();


}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type,DiagramItem::MODE opMode)
{

    DiagramItem item(type,itemMenu,nullptr,opMode);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]
