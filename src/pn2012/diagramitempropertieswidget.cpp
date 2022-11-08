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
 * Implementace widgetu, ktery zobrazuje vlastnosti vybraneho prvku sceny.
 *
 */

#include "diagramitempropertieswidget.h"

DiagramItemPropertiesWidget::DiagramItemPropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn){

    if(settingsIn == NULL || simDataIn == NULL){
        qDebug() << "NULL Parameter when creating DiagramItemPropertiesWidget";
        return;
    }

    // Nastaveni lokalnich promenny objektu
    settings = settingsIn;
    simData = simDataIn;

    // Vytvoreni layoutu
    widgetLayout = new QVBoxLayout;

    // Vytvoreni widgetu pro ruzne typy prvku
    placeWidget = new DiagramPlacePropertiesWidget(settings, simData);
    transitionWidget = new DiagramTransitionPropertiesWidget(settings, simData);
    arcWidget = new DiagramArcPropertiesWidget(settings, simData);

    currentMode = UnknownWidget;
    setLayout(widgetLayout);
}

void DiagramItemPropertiesWidget::setItem(DiagramItem *itemIn){

    /* odstraneni widgetu z layout */
    switch(currentMode){
    case PlaceWidget:
        delete placeWidget;
        break;
    case TransitionWidget:
        delete transitionWidget;
        break;
    case ArcWidget:
        delete arcWidget;
        break;
    default:
        //qDebug() << "Error: Unknown widget type";
        break;
    }

    // Zobrazeni widgetu podle typu vybraneho prvku
    // Zobrazeni jeho vlastnosti
    QString itemTypeString;
    switch(itemIn->type){
    case Place:
        itemTypeString = "Place";
        placeWidget = new DiagramPlacePropertiesWidget(settings, simData);
        placeWidget->setItem((DiagramPlace*)itemIn);
        currentMode = PlaceWidget;
        widgetLayout->addWidget(placeWidget);
        break;
    case Transition:
        itemTypeString = "Transition";
        transitionWidget = new DiagramTransitionPropertiesWidget(settings, simData);
        transitionWidget->setItem((DiagramTransition*)itemIn);
        currentMode = TransitionWidget;
        widgetLayout->addWidget(transitionWidget);
        break;
    case Arc:
        itemTypeString = "Arc";
        arcWidget = new DiagramArcPropertiesWidget(settings, simData);
        arcWidget->setItem((DiagramArc*)itemIn);
        currentMode = ArcWidget;
        widgetLayout->addWidget(arcWidget);
        break;
    case Unknown:
        itemTypeString = "Unknown";
        break;
    }

    //qDebug() << "Showing " << itemTypeString << " property widget";
}

void DiagramItemPropertiesWidget::save()
{

    // Ulozeni vepsanych vlastnosti prvku
    switch(currentMode){
    case PlaceWidget:
        placeWidget->save();
        break;
    case TransitionWidget:
        transitionWidget->save();
        break;
    case ArcWidget:
        arcWidget->save();
        break;
    default:
        //qDebug() << "Error: Unknown widget type";
        break;
    }
}

void DiagramItemPropertiesWidget::reload()
{

    // Znovunacteni vlastnosti prvku a jejich zobrazeni
    switch(currentMode){
    case PlaceWidget:
        placeWidget->reload();
        break;
    case TransitionWidget:
        transitionWidget->reload();
        break;
    case ArcWidget:
        arcWidget->reload();
        break;
    default:
        //qDebug() << "Error: Unknown widget type";
        break;
    }
}
