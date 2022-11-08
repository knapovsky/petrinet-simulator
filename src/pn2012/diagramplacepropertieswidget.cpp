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
 * Implementace widgetu, ktery zobrazuje vlastosti simulacniho mista.
 *
 */

#include "diagramplacepropertieswidget.h"

DiagramPlacePropertiesWidget::DiagramPlacePropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn){

    // Nastaveni promennych objektu
    settings = settingsIn;
    simulationData = simDataIn;

    // Vytvoreni vnitrnich widgetu
    createInsideWidgets();
}

void DiagramPlacePropertiesWidget::createInsideWidgets()
{
    //
    // Labely
    //
    QLabel *id = new QLabel("ID:", this);
    QLabel *desc = new QLabel("Description:", this);
    QLabel *vals = new QLabel("Values:", this);

    //
    // Line Edits
    //
    idE = new QLineEdit(this);
    descE = new QLineEdit(this);
    //valuesE = new QLineEdit(this);
    valuesList = new ListWidget(this);

    // Pridani widgetu do layoutu
    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(id, 0, 0);
    widgetLayout->addWidget(idE, 0, 1);
    widgetLayout->addWidget(desc, 1, 0);
    widgetLayout->addWidget(descE, 1, 1);
    widgetLayout->addWidget(vals, 2, 0);
    widgetLayout->addWidget(valuesList, 2, 1);

    // Nastaveni centralniho layoutu
    setLayout(widgetLayout);
}

void DiagramPlacePropertiesWidget::setItem(DiagramPlace *selectedItemIn)
{
    // Nastaveni vybraneho mista a zobrazeni jeho vlastnosti
    selectedItem = selectedItemIn;
    showProperties();
}

void DiagramPlacePropertiesWidget::save()
{
    // Alias
    setProperties();
}

void DiagramPlacePropertiesWidget::reload()
{
    // Alias
    showProperties();
}

void DiagramPlacePropertiesWidget::showProperties()
{
    // Zobrazeni vlastnosti mista do widgetu
    idE->setText(selectedItem->properties->getID());
    descE->setText(selectedItem->properties->getDescription());
    valuesList->setItem(selectedItem);
}

void DiagramPlacePropertiesWidget::setProperties(){

    // Ulozeni zadanych vlastnosti mista

    if(idE->text().length() < 1 || idE->text().length() > 10){
        settings->getMainWindowPointer()->statusBar()->showMessage("ID length from 1 to 10 characters");
        settings->showAlert("ID length from 1 to 10 characters");
        this->showProperties();
        return;
    }
    selectedItem->properties->id = idE->text();
    selectedItem->properties->description = descE->text();
    //valuesList->save();

    selectedItem->prepareGeometryChangePublic();
    selectedItem->update();
}
