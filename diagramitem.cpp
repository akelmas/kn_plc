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

#include "diagramitem.h"
#include "arrow.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
                         QGraphicsItem *parent,MODE opMode)
    : QGraphicsPathItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    normallyOpen=true;
    operationMode=opMode;
    QPainterPath path;
    QFont diagram_font("Ubuntu Mono Bold",16);

    no_contact.addText(-25,0,diagram_font,"—| |—");

    no_contact.addRect(-35,-35,70,70);

    nc_contact.addText(-25,0,diagram_font,"—|/|—");
    nc_contact.addRect(-35,-35,70,70);

    no_coil.addText(-25,0,diagram_font,"—( )—");
    no_coil.addRect(-35,-35,70,70);

    nc_coil.addText(-25,0,diagram_font,"—(/)—");
    nc_coil.addRect(-35,-35,70,70);


    switch (myDiagramType) {
    case DigitalIn0:
        path=no_contact;
        title="DIN[0]";
        break;
    case DigitalIn1:
        path=no_contact;
        title="DIN[1]";
        break;
    case DigitalIn2:
        path=no_contact;
        title="DIN[2]";
        break;
    case DigitalIn3:
        path=no_contact;
        title="DIN[3]";
        break;
    case DigitalIn4:
        path=no_contact;
        myPath=path;
        title="DIN[4]";
        break;
    case DigitalIn5:
        path=no_contact;
        title="DIN[5]";
        break;
    case DigitalIn6:
        path=no_contact;
        title="DIN[6]";
        break;
    case DigitalIn7:
        path=nc_contact;
        title="DIN[7]";
        break;
    case TIM1:
        path=no_contact;
        title="TIM1";
        break;
    case CNT1:
        path=no_contact;
        title="CNT1";
        break;
    case CNT2:
        path=no_contact;
        title="CNT2";
        break;
    case AUX0:
        path=no_coil;
        title="AUX[0]";
        break;
    case AUX1:
        path=no_coil;
        title="AUX[1]";
        break;
    case AUX2:
        path=no_coil;
        title="AUX[2]";
        break;
    case AUX3:
        path=no_coil;
        title="AUX[3]";
        break;
    case AUX4:
        path=no_coil;
        title="AUX[4]";
        break;
    case AUX5:
        path=nc_coil;
        title="AUX[5]";
        break;
    case AUX6:
        path=no_coil;
        title="AUX[6]";
        break;
    case AUX7:
        path=no_coil;
        title="AUX[7]";
        break;
    case DigitalOut0:
        path=no_coil;
        title="DOUT[0]";
        break;
    case DigitalOut1:
        path=no_coil;
        title="DOUT[1]";
        break;
    case DigitalOut2:
        path=no_coil;
        title="DOUT[2]";
        break;
    case DigitalOut3:
        path=no_coil;
        title="DOUT[3]";
        break;
    case DigitalOut4:
        path=no_coil;
        title="DOUT[4]";
        break;
    case DigitalOut5:
        path=no_coil;
        title="DOUT[5]";
        break;
    case DigitalOut6:
        path=no_coil;
        title="DOUT[6]";
        break;
    case DigitalOut7:
        path=no_coil;
        title="DOUT[7]";
        break;
    case TIM1EN:
        path=no_coil;
        title="TIM1EN";
        break;
    case TIM1STOP:
        path=no_coil;
        title="TIM1STOP";
        break;
    case CNT1SET:
        path=no_coil;
        title="CNT1SET";
        break;
    case CNT2SET:
        path=no_coil;
        title="CNT2SET";
        break;
    case CNT1RESET:
        path=no_coil;
        title="CNT1RST";
        break;
    case CNT2RESET:
        path=no_coil;
        title="CNT2RST";
        break;

    }

    path.addText(QPointF(-32,30),QFont(),title);
    myPath=QPainterPath(path);
    myPolygon=QPolygonF(QRectF(-35,-35,70,70));
    setPen(QPen(Qt::black, 2));
    setPath(path);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

DiagramItem::DiagramItem(const DiagramItem &diagramItem)
{
    myDiagramType=diagramItem.diagramType();
    operationMode=diagramItem.opMode();
    title=diagramItem.title;
    arrows=diagramItem.arrows;
    arrows_in=diagramItem.arrows_in;
    arrows_out=diagramItem.arrows_out;

}
//! [0]

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
//! [3]


//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(70, 70);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));
    painter.translate(35, 35);
    painter.drawPath(myPath);

    return pixmap;
}
//! [4]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {

        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]
void DiagramItem::makeInput(){
    if(normallyOpen){
        myPath=no_contact;
    }

    else{
        myPath=nc_contact;
    }


    myPath.addText(QPointF(-32,30),QFont(),title);

    setPath(myPath);

}

void DiagramItem::makeOutput(){
    if(normallyOpen){
        myPath=no_coil;
    }

    else{
        myPath=nc_coil;
    }


    myPath.addText(QPointF(-32,30),QFont(),title);

    setPath(myPath);
}

bool DiagramItem::operator==(DiagramItem &item)
{
    return myDiagramType==item.diagramType();

}

QString DiagramItem::operator<<(DiagramItem &item)
{
    return item.title;
}
