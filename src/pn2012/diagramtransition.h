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
 * Hlavickovy soubor s definici tridy reprezentujici simulacni prechod a jeho
 * grafickou podobu.
 *
 */

#ifndef DIAGRAMTRANSITION_H
#define DIAGRAMTRANSITION_H

//#include <QPainter>
//#include <QGraphicsItem>
//#include <QGraphicsScene>
//#include <QDebug>
#include "diagramitem.h"

class DiagramTransition : public DiagramItem
{

public:

    /**
      * Konstruktor vytvarejici novy prechod
      *
      * @param id_in ID prechodu
      * @param conditions_in Seznam podminek prechodu
      * @param operations_in Seznam operaci prechodu
      * @param x_in Pozice na ose X
      * @param y_in Pozice na ose Y
      * @param width_in Sirka prechodu
      * @param height_in Vyska prechodu
      * @param color_in Barva prechodu
      * @param index_in Index prechodu - vrstva, ve ktere se ve scene zobrazuje
      * @param description_in Textovy popis prechodu
      * @param *&setting Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return ukazatel na vytvoreny prechod
      */
    DiagramTransition(QString id_in,
                      QList<QString> conditions_in,
                      QList<QString> operations_in,
                      qreal x_in,
                      qreal y_in,
                      qreal width_in,
                      qreal height_in,
                      QColor color_in,
                      int index_in,
                      QString description_in,
                      Settings *&settingsIn,
                      SimulationData *simDataIn);

    /** Urcuje obdelnik obalujici prechod
      * Pro dalsi informace nahlednete do dokumentace Qt
      * @return obdelnik obalujici prechod
      */
    QRectF boundingRect() const;

    /** Vykreslovaci funkce
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
      * Ziska ID prechodu
      * @return Retezec s ID prechodu
      */
    QString getID();

    /**
      * Zpozdeni prechodu
      */
    qint32 delay;

    /**
      * Seznam podminek prechodu
      */
    QList<QString> conditions;

    /**
      * Seznam operaci prechodu
      */
    QList<QString> operations;

    /**
      * Vrati zpozdeni prechodu
      * @return Zpozdeni prechodu
      */
    qint32 getDelay();

    /**
      * Nastavi zpozdeni prechodu
      * @param delayIn Zpozdeni prechodu
      */
    void setDelay(qint32 delayIn);

protected:

    /**
      * Funkce volana pri stisku tlacitka mysi nad prechodem
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
      * Funkce volana pri pusteni tlacitka mysi nad prechodem
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /**
      * Funkce volana pri pohybu s prechodem
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mouseMoveEvent (QGraphicsSceneMouseEvent * event);
};

#endif // DIAGRAMTRANSITION_H
