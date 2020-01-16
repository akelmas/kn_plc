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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QFont>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;
static bool isMoveDone=true;



//! [0]
class DiagramItem : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 15 };
    enum DiagramType {
        DigitalIn0,
        DigitalIn1,
        DigitalIn2,
        DigitalIn3,
        DigitalIn4,
        DigitalIn5,
        DigitalIn6,
        DigitalIn7,
        TIM1,
        CNT1,
        CNT2,
        AUX0,
        AUX1,
        AUX2,
        AUX3,
        AUX4,
        AUX5,
        AUX6,
        AUX7,
        DigitalOut0,
        DigitalOut1,
        DigitalOut2,
        DigitalOut3,
        DigitalOut4,
        DigitalOut5,
        DigitalOut6,
        DigitalOut7,
        TIM1EN,
        TIM1STOP,
        CNT1SET,
        CNT1RESET,
        CNT2SET,
        CNT2RESET


    };
    enum MODE{
        INPUTONLY,
        BOTH,
        OUTPUTONLY,
    };

    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr,MODE opMode=BOTH);
    DiagramItem(const DiagramItem &diagramItem);


    void removeArrow(Arrow *arrow);
    void removeArrows();
    DiagramType diagramType() const { return myDiagramType; }
    MODE opMode() const {return operationMode;}
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const override { return Type;}

    QPainterPath myPath;
    bool normallyOpen;
    QPainterPath no_contact;
    QPainterPath nc_contact;
    QPainterPath no_coil;
    QPainterPath nc_coil;
    QString title;

    QList<Arrow *> arrows;
    QList<Arrow *> arrows_in;

    QList<Arrow *> arrows_out;

    void makeInput();
    void makeOutput();
    bool operator==(DiagramItem &item);
    QString operator<<(DiagramItem &item);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    DiagramType myDiagramType;
    MODE operationMode;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
};
//! [0]

#endif // DIAGRAMITEM_H
