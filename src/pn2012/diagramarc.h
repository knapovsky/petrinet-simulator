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
 * Hlavickovy soubor s definici tridy reprezentujici hranu diagramu.
 *
 */

#ifndef DIAGRAMARC_H
#define DIAGRAMARC_H

//#include <QtCore>
//#include <QtGui>
//#include <QGraphicsItem>
//#include <QDebug>
#include "diagramplace.h"
#include "diagramtransition.h"
#include "diagramitemproperty.h"
#include "diagramitem.h"


/** Trida reprezentujici hranu petriho site
  * @class
  */

class DiagramArc : public DiagramItem
{
public:

    /**
      * Konstruktor vytvarejici novou hranu
      *
      * @param id_in ID hrany
      * @param variable_in Promenna prirazena hrane
      * @param place_in Misto, ktere hrana spojuje
      * @param transition_in Prechod, ktery hrana spojuje
      * @param direction_in Smer hrany, true = place->trans, false = trans->place
      * @param weight_in Vaha hrany
      * @param color_in Barva hrany
      * @param index_in Index hrany - vrstva, ve ktere se ve scene zobrazuje
      * @param description_in Textovy popis hrany
      * @param *&setting Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return ukazatel na vytvorenou hranu
      */
    DiagramArc(QString id_in,
               QString variable_in,
               DiagramPlace* place_in,
               DiagramTransition* transition_in,
               bool direction_in,
               qint32 weight_in,
               QColor color_in,
               int index_in,
               QString description_in,
               Settings *&setting,
               SimulationData *simDataIn
               );

    /** Urcuje obdelnik obalujici hranu
      * Pro dalsi informace nahlednete do dokumentace Qt
      * @return obdelnik obalujici hranu
      */
    QRectF boundingRect() const;

    /** Vykreslovaci funkce
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /** Ziska ukazatel na misto, ktere je pripojene k teto hrane
      * @return ukazatel na misto, ktere je pripojene k teto hrane
      */
    DiagramPlace* getPlace();

    /** Ziska ukazatel na prechod, ktery je pripojen k teto hrane
      * @return ukazatel na prechod, ktery je pripojen k teto hrane
      */
    DiagramTransition* getTransition();

    /** Urci smer hrany true = place->trans, false = trans->place
      * @return smer hrany
      */
    bool getDirection();

    /** Funkce pro ziskani promenne prirazene ke hrane
      * @return retezec s promennou
      */
    QString getVariable();

    /** Funkce pro ziskani vahy hrany
      * @return vaha hrany
      */
    qint32 getWeight();

    /** Nastavuje vahu hrany
      * @arg weight_in vaha
      */
    void setWeight(int weight_in);

    /** Nastavuje promennou asociovanou s hranou
      * @param variable_in Retezec obsahujici promennou
      */
    void setVariable(QString variable_in);

protected:

    /** Funkce volana pri stisku tlacitka mysi nad hranou
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
      * Misto pripojene ke hrane
      */
    DiagramPlace *place;

    /**
      * Prechod pripojeny ke hrane
      */
    DiagramTransition *transition;

    /**
      * Smer hrany
      * true = place->trans, false = trans->place
      */
    bool direction;

    /**
      * Promenna prirazena ke hrane
      */
    QString variable;

    /**
      * Vaha hrany
      */
    qint32 weight;

    /**
      * Index, ktery urcuje, ve ktere vrstve sceny bude hrana zobrazena
      */
    int index;

    /**
      * Indikace stisknuteho tlacitka mysi nad hranou
      */
    bool Pressed;
};

#endif // DIAGRAMARC_H
