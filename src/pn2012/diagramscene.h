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
 * Hlavickovy soubor s definici tridy reprezentujici grafickou scenu simulace.
 *
 */

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

//#include <QGraphicsScene>
//#include <QtCore>
//#include <QtGui>
//#include <QWidget>
#include "settings.h"
#include "diagramplace.h"
#include "diagramtransition.h"
#include "diagramproperties.h"
#include "simulationdata.h"

class Simulation;

class DiagramScene : public QGraphicsScene
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *simDataIn Ukazatel na data simulace
      * @param *modeObjectPointer_in Ukazatel na globalni nastaveni programu
      * @param *simulationPointerIn Ukazatel na simulaci, ktera se v graficke scene zobrazuje
      * @param *DiagramPropertiesIn Ukazatel na widget, ktery zobrazuje vlastnosti vybraneho prvku ve scene
      * @return Ukazatel na novy objekt graficke sceny
      */
    DiagramScene(SimulationData* simDataIn,
                 Settings *modeObjectPointer_in,
                 Simulation *simulationPointerIn,
                 DiagramProperties *DiagramPropertiesIn);

public slots:

    /**
      * Odstrani prvek sceny, ktery je v simData nastaven na odstraneni
      */
    void removeDiagramItem();

    /**
      * Odstrani hrany, ktere jsou v simData nastaveny na odstraneni
      */
    void removeDiagramArcs();

    /**
      * Vlozi prvek, ktery je v simData nastaven na vlozeni do sceny
      */
    void insertDiagramItem();

signals:

    /**
      * Signal, ktery je vyvolan pri kliknuti na scenu
      */
    void mousePressed();

protected:

    /** Funkce volana pri stisku tlacitka mysi nad grafickou scenou
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /** Funkce volana pri pusteni tlacitka mysi nad grafickou scenou
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /** Funkce volana pri pohybu mysi nad grafickou scenou
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /** Funkce volana pri stisku tlacitka klavesnice
      * Obsluhuje vyhradne tlacitko delete pro odstraneni prvku sceny
      * Pro dalsi informace nahlednete do dokumentace Qt
      */
    void keyPressEvent(QKeyEvent *event);

    // Odkaz na Simulation objekt
    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *modeObjectPointer;

    /**
      * Ukazatel na simulaci, ktera je zobrazena ve graficke scene
      */
    Simulation *simulationPointer;

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simData;

    /**
      * Ukazatel na widget, ktery zobrazuje vlastnosti vybraneho prvku sceny
      */
    DiagramProperties *diagramProperties;

    /**
      * Ukazatel, ktery slouzi pro uchovani docasne primky vykreslovane
      * pri tahu mysi se stisknutym levym tlacitkem mysi
      */
    QGraphicsLineItem *line;

    /**
      * Indikace toho, ze je tlacitko mysi stlaceno
      */
    bool pressed;
};

#endif // DIAGRAMSCENE_H
