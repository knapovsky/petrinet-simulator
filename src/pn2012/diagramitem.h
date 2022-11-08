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
 * Hlavickovy soubor s definici tridy, ze ktere dedi vsechny prvky simulacni sceny.
 *
 */

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

//#include <QtCore>
//#include <QtGui>
//#include <QGraphicsItem>
#include "diagramitemproperty.h"
#include "settings.h"


class SimulationData;

/**
  * @enum Typ prvku
  */
enum ItemType {
    Place = 0,
    Transition,
    Arc,
    Unknown
};

class DiagramItem : public QGraphicsItem
{

public:

    /**
      * Konstruktor
      */
    DiagramItem();

    /**
      * Typ prvku
      */
    ItemType type;

    /**
      * Obdelnik ohranicujici prvek sceny
      * Pro vice informaci nahlednete do Qt dokumentace
      */
    QRectF boundingRect();

    /**
      * Urcuje, zda je bod uvnitr obdelniku ohranicujiciho prvek
      * @return Indikace, zda je bod uvnitr, ci nikoliv
      */
    bool isInsideBoundingRect(QPointF point);

    /**
      * Pripravi prvek na zmenu jeho geometrie - pro aktualizaci sceny
      */
    void prepareGeometryChangePublic();

    /**
      * Ukazatel na vlastnosti prvku
      */
    DiagramItemProperty *properties;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na nastaveni simulace, kde se objekt nachazi
      */
    SimulationData *simData;

    /**
      * Pozice X ve scene
      */
    qreal boundX;

    /**
      * Pozice Y ve scene
      */
    qreal boundY;

    /**
      * Indikace, zda byl prvek namacknut
      */
    bool Pressed;
};

#endif // DIAGRAMITEM_H
