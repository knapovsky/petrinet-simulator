/**
 * @file
 * @author Martin Knapovsky <xknapo02@stud.fit.vutbr.cz>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2012, Martin Knapovsky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *   must display the following acknowledgement:
 *    This product includes software developed by the Martin Knapovsky.
 * 4. Neither the name of the Martin Knapovsky nor the
 *    names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MARTIN KNAPOVSKY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MARTIN KNAPOVSKY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 *
 * Implementace simulacniho mista, jeho obsahu a graficke podoby.
 *
 */

#include "diagramplace.h"
#include "diagramarc.h"
#include "simulationdata.h"
#include <QDebug>
#include <iostream>
#include <cmath>

DiagramPlace::DiagramPlace(QString id_in,
                           QList<int> values_in,
                           qreal x_in,
                           qreal y_in,
                           qreal width_in,
                           qreal height_in,
                           QColor color_in,
                           int index_in,
                           QString description_in,
                           Settings *&settingsIn,
                           SimulationData *simDataIn)
{
    // Inicializace
    Pressed = false;

    // Objekt je pohyblivy
    setFlag(ItemIsMovable);

    // Vytvoreni objektu s vlastnostmi mista
    properties = new DiagramItemProperty(id_in, x_in, y_in, width_in, height_in, color_in, index_in, description_in);

    // Nastaveni promennych objektu
    values = values_in;
    settings = settingsIn;
    simData = simDataIn;
    type = Place;
    boundX = x_in;
    boundY = y_in;
}

QRectF DiagramPlace::boundingRect() const
{
    // Tady se bude menit poloha objektu a velikost
    return QRectF(boundX, boundY, properties->width, properties->height);
}

void DiagramPlace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    // Nastaveni barvy stlaceneho objektu
    if(Pressed){
        brush.setColor(Qt::red);
    }
    else{
        brush.setColor(properties->getColor());
    }
    painter->setBrush(brush);

    // Vypocet promennych potrebnych pro spravne vykreslovani textu
    qreal squareA = sqrt((pow(properties->getWidth(), 2)/2));
    qreal squareDiff = properties->getWidth() - squareA;

    // Nastaveni pisma
    QFont f("Helvetica", 12);
    painter->setFont(f);
    QFontMetrics fm = painter->fontMetrics();

    // Nastaveni textu
    // Text ve vypisuje podle toho, zda se vejde do
    // Obdelniku, ktery je vepsany elipse mista
    QString valuesString = "";
    int j = 1;
    int thisLineWidth = 0;

    // Pruchod hodnotami mista
    for(QList<int>::iterator i = values.begin(); i != values.end(); i++){

        // Pri malem mistu se hodnoty nevypisuji...mensi jak 50
        if(squareA < 50){valuesString = ""; break; }

        // Delka retezce s aktualni hodnotou je delsi nez sirka vepsaneho obdelniku
        if(thisLineWidth+fm.width(QString("%1, ").arg(*i)) >= squareA ){

            j++;
            int height = fm.height();
            int currentHeight = j * height;

            // Odradkovani
            if(qreal(currentHeight) < squareA){
                valuesString.append(",\n");
                // Reset delky radku
                thisLineWidth = 0;
            }
            else{
                // Text je priliz dlouhy a "..." se do mista vejdou
                if(fm.width("...") < squareA){
                    valuesString = "...";
                }
                else{
                    // Text je priliz dlouhy a "..." se do mista nevejdou - nic se nezobrazi
                    valuesString = "";
                }
            }
        }

        // Prvni hodnota na radku - neobsahuje carku
        if(i == values.begin() || thisLineWidth == 0){

            //valuesString.append(properties->getID());
            valuesString.append(QString("%1").arg(*i));
            thisLineWidth += fm.width(QString("%1").arg(*i));
        }

        // Dalsi hodnota na radku - obsahuje carku
        else{

            valuesString.append(QString(", %1").arg(*i));
            thisLineWidth += fm.width(QString(", %1").arg(*i));
        }
    }

    // Nastaveni pera podle toho, zda je objekt zvolen
    if(settings->selectedItem == this){
      painter->setPen(settings->selectedPen);
    }
    else{
        painter->setPen(settings->notSelectedPen);
    }

    // Nastaveni vrstvy, ve ktere se misto zobrazuje
    this->setZValue(properties->index);

    // Vykresleni elipsy mista
    painter->drawEllipse(rec.x(), rec.y(), rec.width(), rec.height());

    // Vykresleni textu s obsahem mista
    painter->drawText(QRectF(rec.x() + squareDiff/2, rec.y() + squareDiff/2, squareA, squareA),
                      Qt::AlignCenter, valuesString);
}

QString DiagramPlace::getID()
{
    return properties->getID();
}

void DiagramPlace::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Misto namacknuto
    Pressed = true;

    // Zobrazeni vlastnosti mista
    settings->setSelectedItem(this);

    // Update Sceny
    update();

    // Delegace eventu
    QGraphicsItem::mousePressEvent(event);
}

void DiagramPlace::mouseMoveEvent(QGraphicsSceneMouseEvent *event){

    // Priprava na aktualizaci sceny
    prepareGeometryChange();

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();

    // Aktualizace polohy
    this->properties->x = rec.x() + this->pos().x();
    this->properties->y = rec.y() + this->pos().y();

    // Aktualizace tohoto mista
    update();
    settings->updateItem(this);

    // Aktualizace hran s mistem spojenych
    QList<DiagramArc*>::iterator iter;
    for(iter = simData->diagramArcs.begin(); iter != simData->diagramArcs.end(); iter++){
        (*iter)->prepareGeometryChangePublic();
        (*iter)->update();
    }

    QGraphicsItem::mouseMoveEvent(event);

}


void DiagramPlace::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Priprava na aktualizaci sceny
    prepareGeometryChange();

    // Misto neni namacknuto
    Pressed = false;

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();

    // Aktualizace polohy
    this->properties->x = rec.x() + this->pos().x();
    this->properties->y = rec.y() + this->pos().y();

    // Aktualizace tohoto mista
    update();
    settings->updateItem(this);

    // Aktualizace hran s mistem spojenych
    QList<DiagramArc*>::iterator iter;
    for(iter = simData->diagramArcs.begin(); iter != simData->diagramArcs.end(); iter++){
        (*iter)->prepareGeometryChangePublic();
        (*iter)->update();
    }

    // Delegace eventu
    QGraphicsItem::mouseReleaseEvent(event);
}
