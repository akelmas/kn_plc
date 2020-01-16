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

#include "diagramscene.h"
#include "arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::DigitalIn0;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type,DiagramItem::MODE opMode)
{
    myItemType = type;
    myItemMode=opMode;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (myMode) {
    case InsertItem:
        item = new DiagramItem(myItemType, myItemMenu,nullptr,myItemMode);
        item->setBrush(myItemColor);
        addItem(item);

        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;
        //! [6] //! [7]
    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
        break;
        //! [7] //! [8]
    case InsertText:
        textItem = new DiagramTextItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);
        break;
        //! [8] //! [9]

    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(itemAt(mouseEvent->scenePos(),QTransform()));
        isMoveDone=false;
        if(startItem!=nullptr){
            startItem->setZValue(10);

            startItem->setPos(mouseEvent->scenePos());

            foreach(Arrow* item,startItem->arrows){
                if(item->startItem()->x()>=item->endItem()->x()){
                    item->startItem()->removeArrow(item);
                    item->endItem()->removeArrow(item);
                    delete item;
                }
            }

        }


        mouseEvent->accept();
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setBackgroundBrush(QPixmap(":/images/scene_background.svg"));


    QPointF mouse_pos(mouseEvent->scenePos());
    if(myMode==MoveItem){
        QList<QGraphicsItem *> startItems = items(mouse_pos);
        if(!startItems.isEmpty()){
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            isMoveDone=false;
            if(startItem!=nullptr){
                startItem->setZValue(0);
                int curX=int(mouse_pos.x()/100);
                int curY=int(mouse_pos.y()/100);
                startItem->setPos(curX*100+50,curY*100+50);
                foreach(Arrow* item,startItem->arrows){
                    item->updatePosition();
                    item->update();
                }
                if(curX>=4){
                    if(startItem->opMode()>0){
                        startItem->makeOutput();

                    }
                    else{
                        deleteItem();
                    }

                }
                else{
                    if(startItem->opMode()<2){

                        startItem->makeInput();
                    }
                    else{
                        deleteItem();
                    }
                }
            }
        }



    }

    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
        //! [11] //! [12]



        if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == DiagramItem::Type &&
                endItems.first()->type() == DiagramItem::Type &&
                startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            if(startItem->x()<endItem->x()){
                Arrow *arrow = new Arrow(startItem, endItem);
                arrow->setColor(myLineColor);
                startItem->addArrow(arrow);
                startItem->arrows_out.append(arrow);
                endItem->arrows_in.append(arrow);
                endItem->addArrow(arrow);

                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }


        }
    }
    //! [12] //! [13]
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    //event->accept();
    event->ignore();
}
//! [13]
void DiagramScene::deleteItem(){
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == Arrow::Type) {
            removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->startItem()->arrows_out.removeOne(arrow);
            arrow->endItem()->removeArrow(arrow);
            arrow->endItem()->arrows_in.removeOne(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item,selectedItems()) {
        if (item->type() == DiagramItem::Type)
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        removeItem(item);
        delete item;
    }
}
//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {

        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]

QString DiagramScene:: generateBooleanExpressions()
{
    outputList.clear();

    foreach (QGraphicsItem *item, items()) {
        if(item->type()==DiagramItem::Type)
            if(qgraphicsitem_cast<DiagramItem*> (item)->x()/100>4){
                outputList.append(qgraphicsitem_cast<DiagramItem*> (item));
            }
    }

    QString result;
    foreach(DiagramItem *item,outputList){
        result.append(item->title);



        result.append("=");
        if(!item->normallyOpen){
            result.append("!");
        }
        result.append("(");
        result.append(generateBooleanExp(item,0));
        result.append(")");
        result.append(";\n");

    }
    return result;
}


QString DiagramScene::generateBooleanExp(DiagramItem *item,int step){
    if(item->arrows_in.size()==0){
        return (item->normallyOpen)?item->title:"!"+item->title;
    }
    else if(item->arrows_in.size()==1){
        QString result;
        if(step>0){
            if(!item->arrows_in.first()->endItem()->normallyOpen){
                result.append("!");
            }
            result.append(item->arrows_in.first()->endItem()->title);
            result.append("&");
        }


        result.append(generateBooleanExp(item->arrows_in.first()->startItem(),step+1));

        return result;
    }
    else{

        QString result;
        if(step>0){
            if(!item->normallyOpen){
                result.append("!");
            }
            result.append(item->title);
            result.append("&");
        }

        int arrow_count=item->arrows_in.size()-1;

        result.append("(");
        foreach(Arrow *arrow,item->arrows_in){
            result.append("(");

            result.append(generateBooleanExp(arrow->startItem(),step+1));

            result.append(")");
            if(arrow_count>0){
                result.append("|");
            }


            arrow_count--;

        }
        result.append(")");

        return result;
    }
}
