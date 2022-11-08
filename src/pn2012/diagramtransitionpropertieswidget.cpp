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
 * Implementace widgetu, ktery zobrazuje specificke vlastnosti prechodu
 * a umoznuje jejich editaci.
 *
 */

#include "diagramtransitionpropertieswidget.h"

DiagramTransitionPropertiesWidget::DiagramTransitionPropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn)
{
    // Nastaveni promennych objektu
    settings = settingsIn;
    simulationData = simDataIn;

    // Vytvoreni vnitrnich widgetu - obsahu tohoto widgetu
    createInsideWidgets();
}

void DiagramTransitionPropertiesWidget::createInsideWidgets()
{

    // Vytvari obsah widgetu

    //
    // LABELS
    //
    QLabel *id = new QLabel("ID:", this);
    QLabel *desc = new QLabel("Description:", this);
    QLabel *conditions = new QLabel("Conditions:", this);
    QLabel *operations = new QLabel("Operations:", this);
    //QLabel *delay = new QLabel("Delay:", this);

    //
    // LINE EDITS
    idE = new QLineEdit(this);
    descE = new QLineEdit(this);
    //delayE = new QLineEdit(this);

    // Widgety pro zobrazeni a editaci podminek a operaci
    condList = new DiagramTransitionCondWidget(this);
    opList = new DiagramTransitionOpWidget(this);

    // Pridani widgetu do layoutu
    QHBoxLayout *propLayout = new QHBoxLayout;
    propLayout->addWidget(id);
    propLayout->addWidget(idE);
    propLayout->addWidget(desc);
    propLayout->addWidget(descE);
    //propLayout->addWidget(delay);
    //propLayout->addWidget(delayE);

    // Pridani Widgetu pro zobrazeni podminek a operaci do
    // dalsiho layoutu
    QHBoxLayout *listsLayout = new QHBoxLayout;
    listsLayout->addWidget(conditions);
    listsLayout->addWidget(condList);
    listsLayout->addWidget(operations);
    listsLayout->addWidget(opList);

    // Pridani layoutu do hlavniho layoutu
    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addLayout(propLayout);
    widgetLayout->addLayout(listsLayout);

    // Nastaveni hlavniho layoutu
    setLayout(widgetLayout);
}

void DiagramTransitionPropertiesWidget::setItem(DiagramTransition *selectedItemIn)
{
    // Nastavi zvoleny prechod
    selectedItem = selectedItemIn;

    // Zobrazi jeho vlastnosti
    showProperties();
}

void DiagramTransitionPropertiesWidget::save()
{
    // Ulozi zadane vlastnosti
    setProperties();
}

void DiagramTransitionPropertiesWidget::reload()
{
    // Zobrazi vlastnosti prechodu ve widgetu
    showProperties();
}

void DiagramTransitionPropertiesWidget::showProperties()
{
    // Zobrazi vlastnosti prechodu ve widgetu
    idE->setText(selectedItem->properties->getID());
    descE->setText(selectedItem->properties->getDescription());
    condList->setItem(selectedItem);
    opList->setItem(selectedItem);
    //delayE->setText(QString("%1").arg(selectedItem->getDelay()));
}

void DiagramTransitionPropertiesWidget::setProperties(){

    // Ulozi zadane vlastnosti

    // ID - omezeni na 1 az 10 znaku
    if(idE->text().length() < 1 || idE->text().length() > 10){

        settings->getMainWindowPointer()->statusBar()->showMessage("ID length from 1 to 10 characters");
        settings->showAlert("ID length from 1 to 10 characters");
        this->showProperties();
        return;
    }

    // Delay - omezeni na 1 az 1000
    /*if(delayE->text().toInt() < 1 || delayE->text().toInt() > 1000){
        settings->getMainWindowPointer()->statusBar()->showMessage("Set delay length from 1 to 1000");
        settings->showAlert("Set delay length from 1 to 1000");
        this->showProperties();
        return;
    }*/

    // ID
    selectedItem->properties->id = idE->text();

    // Description
    selectedItem->properties->description = descE->text();

    // Delay
    //selectedItem->setDelay(delayE->text().toInt());

    // Aktualizace prechodu ve scene
    selectedItem->prepareGeometryChangePublic();
    selectedItem->update();
}

