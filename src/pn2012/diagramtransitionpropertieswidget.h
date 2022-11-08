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
 * Hlavickovy soubor s definici tridy, ktera predstavuje widget zobrazujici
 * specificke vlastnosti prechodu a umoznuje jejich editaci.
 *
 */

#ifndef DIAGRAMTRANSITIONPROPERTIESWIDGET_H
#define DIAGRAMTRANSITIONPROPERTIESWIDGET_H

//#include <QtCore>
//#include <QtGui>
#include "diagramtransition.h"
#include "settings.h"
#include "simulationdata.h"
#include "diagramtransitioncondwidget.h"
#include "diagramtransitionopwidget.h"

class Simulation;

class DiagramTransitionPropertiesWidget : public QWidget{

public:

    /**
      * Konstruktor
      * @param *&settingsIn Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return Ukazatel na nove vytvoreny objekt widgetu
      */
    DiagramTransitionPropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn);

    /**
      * Zobrazi vlastnosti prechodu ve widgetu
      */
    void showProperties();

    /**
      * Ulozi vlastnosti zadane do widgetu k vybranemu prechodu
      */
    void setProperties();

    /**
      * Nastavi zobrazovany prechod
      * @param *selectedItemIn Vybrany prechod
      */
    void setItem(DiagramTransition *selectedItemIn);

    /**
      * Alias k setProperties()
      */
    void save();

    /**
      * Alias k showProperties()
      */
    void reload();

private:

    /**
      * Vytvori obsah widgetu
      */
    void createInsideWidgets();

    /**
      * Hlavni Layout widgetu
      */
    QGridLayout *layout;

    /**
      * Ukazatel na simulaci, k niz je tento widget pridruzen
      */
    Simulation *simulationPointer;

    /**
      * Ukazatel na ID line edit
      */
    QLineEdit *idE;

    /**
      * Ukazatel na Description line edit
      */
    QLineEdit *descE;

    /**
      * Ukazatel na Delay line edit
      */
    //QLineEdit *delayE;

    /**
      * Ukazatel na Widget zobrazujici podminky prechodu
      */
    DiagramTransitionCondWidget *condList;

    /**
      * Ukazatel na Widget zobrazujici operace prechodu
      */
    DiagramTransitionOpWidget *opList;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simulationData;

    /**
      * Ukazateln na vybrany prechod
      */
    DiagramTransition *selectedItem;
};

#endif // DIAGRAMTRANSITIONPROPERTIESWIDGET_H
