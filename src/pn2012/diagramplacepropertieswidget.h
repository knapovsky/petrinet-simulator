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
 * Hlavickovy soubor s definici tridy, ktera reprezentuje widget zobrazujici
 * vlastnosti simulacniho mista.
 *
 */

#ifndef DIAGRAMPLACEPROPERTIESWIDGET_H
#define DIAGRAMPLACEPROPERTIESWIDGET_H

//#include <QtCore>
//#include <QtGui>
#include "diagramplace.h"
#include "settings.h"
#include "simulationdata.h"
#include "listwidget.h"

class Simulation;

class DiagramPlacePropertiesWidget : public QWidget
{

public:

    /**
      * Konstruktor
      * @param *&settingsIn Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return Ukazatel na nove vytvoreny objekt
      */
    DiagramPlacePropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn);

    /**
      * Zobrazi vlastnosti vybraneho mista
      */
    void showProperties();

    /**
      * Nastavi zadane vlastnosti vybraneho mista
      */
    void setProperties();

    /**
      * Zobrazi vlastnosti vybraneho mista
      * @param *selectedItemIn Vybrane misto
      */
    void setItem(DiagramPlace *selectedItemIn);

    /**
      * Alias pro setProperties()
      */
    void save();

    /**
      * Znovu nacte puvodni hodnoty mista a zobrazi je
      */
    void reload();

private:

    /**
      * Vytvori vnitrni widgety
      */
    void createInsideWidgets();

    /**
      * Hlavni layout tohoto widgetu
      */
    QGridLayout *layout;

    /**
      * Ukazatel na simulace, ve ktere se misto nachazi
      */
    Simulation *simulationPointer;

    /**
      * Ukazatel na ID LineEdit
      */
    QLineEdit *idE;

    /**
      * Ukazatel na Description LineEdit
      */
    QLineEdit *descE;

    /**
      * Ukazatel na Values LineEdit
      */
    QLineEdit *valuesE;

    /**
      * Ukazatel na Widget, ktery zobrazuje ohodnoceni vybraneho mista
      */
    ListWidget *valuesList;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simulationData;

    /**
      * Ukazatel na vybrane misto
      */
    DiagramPlace *selectedItem;
};

#endif // DIAGRAMPLACEPROPERTIESWIDGET_H
