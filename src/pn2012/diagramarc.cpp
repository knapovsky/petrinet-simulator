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
 * Implementace simulacni hrany
 *
 */
 
#include "diagramarc.h"

DiagramArc::DiagramArc(QString id_in,
                       QString variable_in,
                       DiagramPlace* place_in,
                       DiagramTransition* transition_in,
                       bool direction_in,
                       qint32 weight_in,
                       QColor color_in,
                       int index_in,
                       QString description_in,
                       Settings *&settingsIn,
                       SimulationData *simDataIn){


    // Nastaveni promenny objektu
    variable = variable_in;
    place = place_in;
    transition = transition_in;
    weight = weight_in;
    direction = direction_in;
    settings = settingsIn;
    type = Arc;
    simData = simDataIn;
    index = index_in;

    // Vytvoreni objektu s vlastnostmi hrany
    properties = new DiagramItemProperty(id_in, 0, 0, 0, 0, color_in, index_in, description_in);
}

QRectF DiagramArc::boundingRect() const{

    // Nastaveni pozic bodu, ke kterym se hrana vztahuje
    qreal transMiddleY = transition->properties->y + transition->properties->height/2;
    qreal placeMiddleY = place->properties->y + place->properties->height/2;
    qreal placeMiddleX = place->properties->x + place->properties->width/2;

    /* Obdelnik se lisi podle vzajemne pozice mista a hrany */
    /* Prechod je vpravo od mista */
    if(placeMiddleX <= transition->properties->x){
        /* Misto je vlevo a nahore -> Qt ma otocenou osu Y */
        if(placeMiddleY <= transMiddleY){
            //qDebug() << "Place up left";
            return QRectF(placeMiddleX,
                          placeMiddleY,
                          qAbs(transition->properties->x - placeMiddleX),
                          qAbs(transMiddleY - placeMiddleY));
        }
        /* Misto je vlevo a dole */
        else{
            //qDebug() << "Place down left";
            return QRectF(placeMiddleX,
                          transMiddleY,
                          qAbs(transition->properties->x - placeMiddleX),
                          qAbs(transMiddleY - placeMiddleY));
        }
    }
    /* Prechod je vlevo */
    else{
        /* Prechod je vlevo a nahore */
        if(placeMiddleY >= transMiddleY){
            //qDebug() << "Place down right";
            return QRectF(transition->properties->x,
                          transMiddleY,
                          qAbs(transition->properties->x - placeMiddleX),
                          qAbs(transMiddleY - placeMiddleY));
        }
        /* Prechod je vlevo a dole */
        else{
            //qDebug() << "Place up right";
            return QRectF(transition->properties->x,
                          placeMiddleY,
                          qAbs(transition->properties->x - placeMiddleX),
                          qAbs(transMiddleY - placeMiddleY));
        }
    }

}

void DiagramArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{    

    // Nastaveni pera podle toho, zda je objekt vybran
    if(settings->selectedItem == this){
      painter->setPen(settings->selectedPen);
    }
    else{
        painter->setPen(settings->notSelectedPen);
    }

    // Ziskani obdelniku obalujiciho hranu
    QRectF rec = boundingRect();

    //qDebug() << QString("Rectangle of ARC %1 %2 %3 %4 %5").arg(this->properties->id).arg(rec.x()).arg(rec.y()).arg(rec.x()+rec.width()).arg(rec.y()+rec.height());

    // Nastaveni pera
    QPen newPen;
    newPen.setWidth(4);
    newPen.setColor(this->properties->color);
    painter->setPen(newPen);
    this->setZValue(index);

    // Nastaveni pozic dulezitych pro vykreslovani hrany
    qreal transMiddleY = transition->properties->y + transition->properties->height/2;
    qreal placeMiddleY = place->properties->y + place->properties->height/2;
    qreal placeMiddleX = place->properties->x + place->properties->width/2;
    qreal transRightX = transition->properties->x + transition->properties->width;
    //qreal transLeftX = transition->properties->x;

    // Nastaveni popisu hrany
    QString arcString = this->variable;
    arcString.append(":");
    arcString.append(QString("%1").arg(this->weight));

    QFont f("Helvetica", 12);
    painter->setFont(f);
    QFontMetrics fm = painter->fontMetrics();

    // Nastaveni Brushe
    painter->setBrush(this->properties->color);

    // Vypocet uhlu, prepony a goniometrickych funkci pro vykresleni hrany
    qreal tan = rec.height()/rec.width();
    qreal prepona = qSqrt(qPow(rec.height(), 2)+ qPow(rec.width(), 2));
    qreal sin = rec.height()/prepona;
    qreal cos = rec.width()/prepona;
    qreal angle = qAtan(tan);
    //qreal angleDeg = (180*angle)/3.1415;
    QPointF translationDestination;

    //qDebug() << "prepona: " << prepona << " tan: " << tan << " sin: " << sin << " cos: " << cos << " angle: " << angle << " angleDeg: " << angleDeg;

    // Vypocet posunu bodu
    qreal widthAdjust = cos * place->properties->width/2;
    qreal heightAdjust = sin * place->properties->height/2;

    if(place->properties->y >= transition->properties->y){
        heightAdjust = -heightAdjust;
    }

    //qDebug() << QString("Sin: %1, Cos: %2").arg(sin).arg(cos);

    /* Vykresleni hrany podle vzajemne pozice mista a hrany */
    /* TRUE - PLACE -> TRANSITION */
    if(direction == true){
        /* Misto je vlevo */
        if(place->properties->x <= transition->properties->x){

            translationDestination = QPointF(transition->properties->x, transMiddleY);
            painter->drawLine(placeMiddleX + widthAdjust,
                              placeMiddleY + heightAdjust,
                              transition->properties->x,
                              transMiddleY);
            /* Misto je dole */
            if(place->properties->y > transition->properties->y){
                angle = -angle;
            }
            else{
                /* tady uhel zustava */
            }
        }
        /* Misto je vpravo */
        else{

            translationDestination = QPointF(transRightX, transMiddleY);
            painter->drawLine(placeMiddleX  - widthAdjust,
                              placeMiddleY + heightAdjust,
                              transRightX, transMiddleY);
            /* Misto je dole */
            if(place->properties->y > transition->properties->y){
                angle = - ((M_PI/2 - angle) + M_PI/2);
            }
            else{
                angle = (M_PI/2 - angle) + M_PI/2;
            }


        }
    }

    else{
        /* Misto je vlevo */
        if(place->properties->x <= transition->properties->x){

            translationDestination = QPointF(placeMiddleX + widthAdjust, placeMiddleY + heightAdjust);
            painter->drawLine(placeMiddleX + widthAdjust,
                              placeMiddleY + heightAdjust,
                              transition->properties->x, transMiddleY);
            /* Misto je dole */
            if(place->properties->y > transition->properties->y){
                /* tady uhel zustava */
                angle =  ((M_PI/2 - angle) + M_PI/2);
            }
            else{
                angle = - ((M_PI/2 - angle) + M_PI/2);
            }

        }
        /* Misto je vpravo */
        else{
            translationDestination = QPointF(placeMiddleX  - widthAdjust, placeMiddleY + heightAdjust);
            painter->drawLine(placeMiddleX  - widthAdjust,
                              placeMiddleY + heightAdjust,
                              transRightX, transMiddleY);
            if(place->properties->y > transition->properties->y){

            }
            else{
                angle = -angle;
            }
        }
    }

    /* Body trojuhelniku - hlavicka sipky */
    QPointF points[3] = {
         QPointF(0.0, 0.0),
         QPointF(-10.0, -5.0),
         QPointF(-10.0, 5.0)
     };

    /* Nejdrive se provadi rotace */
    int i = 0;
    for(i = 0; i < 3; i++){
        float x = points[i].x();
        float y = points[i].y();

        QPointF newPoint;
        newPoint.setX(x*qCos(angle) - y*qSin(angle));
        newPoint.setY(y*qCos(angle) + x*qSin(angle));

        points[i].setX(newPoint.x());
        points[i].setY(newPoint.y());
    }

    /* Potom se provadi translace */
    for(i = 0; i < 3; i++){
        points[i].setX(points[i].x() + translationDestination.x());
        points[i].setY(points[i].y() + translationDestination.y());
    }

    /* Vykresleni trojuhelniku */
    painter->drawPolygon(points, 3);

    /* Vykresleni popisu hrany */
    painter->drawText(rec.x() + rec.width()/2 - fm.width(arcString)/2,
                      rec.y() + rec.height()/2 - 1.5*fm.height(),arcString);
}

DiagramPlace *DiagramArc::getPlace()
{
    return place;
}

DiagramTransition *DiagramArc::getTransition()
{
    return transition;
}

bool DiagramArc::getDirection()
{
    return direction;
}

QString DiagramArc::getVariable()
{
    return variable;
}

qint32 DiagramArc::getWeight()
{
    return weight;
}

void DiagramArc::setWeight(int weight_in)
{
    this->weight = weight_in;
}

void DiagramArc::setVariable(QString variable_in)
{
    this->variable = variable_in;
}

void DiagramArc::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Hrana byla vybrana
    Pressed = true;

    // Zobrazeni Dialogu pro nastaveni hrany
    settings->setSelectedItem(this);

    // Aktualizace hrany
    update();

    // Delegace eventu
    QGraphicsItem::mousePressEvent(event);
}
