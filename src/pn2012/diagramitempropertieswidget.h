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
 * Hlavickovy soubor s definici tridy, ktera slouzi k zobrazeni vlastnosti prvku sceny.
 *
 */

#ifndef DIAGRAMITEMPROPERTIESWIDGET_H
#define DIAGRAMITEMPROPERTIESWIDGET_H

//#include <QtCore>
//#include <QtGui>
//#include <QWidget>
#include "settings.h"
#include "simulationdata.h"
#include "diagramitem.h"
#include "diagramplacepropertieswidget.h"
#include "diagramtransitionpropertieswidget.h"
#include "diagramarcpropertieswidget.h"

/**
  * @enum Typ Widgetu
  */
enum WidgetModes{
    PlaceWidget,
    TransitionWidget,
    ArcWidget,
    UnknownWidget
};

class DiagramItemPropertiesWidget : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *&settingsIn Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na simulacni data
      * @return Ukazatel na nove vytvoreny objekt
      */
    DiagramItemPropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn);

    /**
      * Zobrazi vlastnosti vybraneho prvku sceny
      */
    void setItem(DiagramItem *itemIn);

public slots:

    /**
      * Ulozi vepsane vlastnosti prvku
      */
    void save();

    /**
      * Nacte puvodni vlastnosti prvku a zobrazi je.
      */
    void reload();

private:

    /**
      * Centralni Layout widgetu
      */
    QVBoxLayout *widgetLayout;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simData;

    /**
      * Aktualni mod widgetu
      */
    WidgetModes currentMode;

    /**
      * Ukazatel na vybrane misto
      */
    DiagramPlacePropertiesWidget *placeWidget;

    /**
      * Ukazatel na vybrany prechod
      */
    DiagramTransitionPropertiesWidget *transitionWidget;

    /**
      * Ukazatel na vybranou hranu
      */
    DiagramArcPropertiesWidget *arcWidget;
};

#endif // DIAGRAMITEMPROPERTIESWIDGET_H
