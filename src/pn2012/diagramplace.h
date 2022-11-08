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
 * Hlavickovy soubor s definici tridy, ktera reprezentuje simulacni misto a jeho
 * grafickou podobu.
 *
 */

#ifndef DIAGRAMPLACE_H
#define DIAGRAMPLACE_H

//#include <QPainter>
//#include <QGraphicsItem>
//#include <QDebug>
#include "diagramitemproperty.h"
#include "diagramitem.h"
#include "settings.h"

class DiagramPlace : public DiagramItem
{

public:

    /**
      * Konstruktor vytvarejici nove misto
      *
      * @param id ID hrany
      * @param values_in Seznam hodnot urcujici obsah mista
      * @param x_in Pozice na ose X
      * @param y_in Pozice na ose Y
      * @param width_in Sirka mista
      * @param height_in Vyska mista
      * @param color_in Barva mista
      * @param index_in Index mista - vrstva, ve ktere se ve scene zobrazuje
      * @param description_in Textovy popis mista
      * @param *&setting Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return ukazatel na vytvorene misto
      */
    DiagramPlace(QString id,
                 QList<int> values_in,
                 qreal x_in,
                 qreal y_in,
                 qreal width_in,
                 qreal height_in,
                 QColor color_in,
                 int index_in,
                 QString description_in,
                 Settings *&settingsIn,
                 SimulationData *simDataIn);

    /** Urcuje obdelnik obalujici misto
      * Pro dalsi informace nahlednete do dokumentace Qt
      * @return obdelnik obalujici misto
      */
    QRectF boundingRect() const;

    /** Vykreslovaci funkce
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
      * Ziska ID mista
      * @return Retezec s ID mista
      */
    QString getID();

    /**
      * Seznam s ohodnocenim mista
      */
    QList<int> values;

    /**
      * Urcuje, zda je bod uvnitr obdelniku ohranicujiciho prvek
      * @return Indikace, zda je bod uvnitr, ci nikoliv
      */
    bool isInsideBoundingRect(QPointF point);

protected:

    /**
      * Funkce volana pri stisku tlacitka mysi nad mistem
      * Pro dalsi informace nahlednete do Qt dokumentace
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
      * Funkce volana pri uvolneni tlacitka mysi nad mistem
      * Pro dalsi informace nahlednete do Qt dokumentace
      */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /**
      * Funkce volana pri pohybu s mistem
      * Pro dalsi informace nahlednete do Qt dokumentace
      */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // DIAGRAMPLACE_H
