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
 * Trida reprezentujici widget, ktery slouzi k zobrazeni a nastaveni vlastnosti
 * vybraneho prvku sceny
 *
 */

#ifndef DIAGRAMPROPERTIES_H
#define DIAGRAMPROPERTIES_H

//#include <QTableWidget>
//#include <QtCore>
//#include <QtGui>
#include "simulationdata.h"
#include "diagramitempropertieswidget.h"
#include "diagramitemstylewidget.h"
#include "settings.h"

class DiagramItem;

class DiagramProperties : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *simDataIn Ukazatel na simulacni data
      * @param *&settingsIn Ukazatel na globalni nastaveni programu
      * @return Ukazatel na nove vytvoreny objekt
      */
    DiagramProperties(SimulationData* simDataIn, Settings *&settingsIn);

    /**
      * Nastavi prvek, jehoz vlastnosti maji byt zobrazeny
      * @param *itemIn Vybrany prvek
      */
    void setItem(DiagramItem* itemIn);

signals:

    /**
      * Signal, ktery je vyslan pri odstraneni prvku ze sceny
      */
    void itemRemoved();

    /**
      * Signal, ktery je vyslan pri aktualizaci prvku sceny
      */
    void itemUpdated();

public slots:

    /**
      * Schova tento widget resp. odstrani ho
      */
    void hide();

    /**
      * Aktualizuje scenu
      */
    void updateScene();

private:

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simData;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na widget zobrazujici specificke vlastnosti
      * vybraneho prvku.
      */
    DiagramItemPropertiesWidget *itemProp;

    /**
      * Ukazatel na widget zobrazujici spolecne vlastnosti
      * vybraneho prvku
      */
    DiagramItemStyleWidget *itemStyle;

    /**
      * Ukazatel na OK button widgetu
      */
    QPushButton *ok;

    /**
      * Ukazatel na Cancel button widgetu
      */
    QPushButton *cancel;

    /**
      * Ukazatel na hlavni layout tohoto widgetu
      */
    QVBoxLayout *centralLayout;

    /**
      * Ukazatel na Layout specifickych vlastnosti vybraneho prvku
      */
    QHBoxLayout *newOne;

    /**
      * Ukazatel na Layout spolecnych vlastnosti vybraneho prvku
      */
    QHBoxLayout *nextOne;

    /**
      * Ukazatel na aktualne vybrany prvek sceny
      */
    DiagramItem *currentlySelectedItem;
};

#endif // DIAGRAMPROPERTIES_H
