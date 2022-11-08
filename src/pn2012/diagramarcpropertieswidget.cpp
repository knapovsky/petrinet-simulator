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
 * Implementace widgetu zobrazujiciho vlastnosti hrany
 *
 */

#include "diagramarcpropertieswidget.h"

DiagramArcPropertiesWidget::DiagramArcPropertiesWidget(Settings *&settingsIn, SimulationData *simDataIn)
{
    // Nastaveni lokalnich promennych
    settings = settingsIn;
    simData = simDataIn;

    // Vytvoreni widgetu
    createInsideWidgets();
}

void DiagramArcPropertiesWidget::createInsideWidgets()
{
    //
    // Labely
    //
    QLabel *id = new QLabel("ID:", this);
    QLabel *desc = new QLabel("Description:", this);
    QLabel *from = new QLabel("From", this);
    QLabel *to = new QLabel("To", this);
    QLabel *weight = new QLabel("Weight", this);
    QLabel *variable = new QLabel("Variable", this);

    //
    // Line Edits
    //
    idE = new QLineEdit(this);
    /* Maximalne 10 znaku na ID */
    //idE->setInputMask("nnnnnnnnnn");
    descE = new QLineEdit(this);
    /* From a To jsou vypnuty */
    fromE = new QLineEdit(this);
    toE = new QLineEdit(this);
    weightE = new QLineEdit(this);
    /* Vaha hrany max 999 - nelze zadat zapornou hodnotu */
    //weightE->setInputMask("000");
    variableE = new QLineEdit(this);
    /* Promenna max 3 alfanum znaky */
    //variableE->setInputMask("nnn");

    // Vypnuti vkladani na prvcich from a to
    fromE->setDisabled(true);
    toE->setDisabled(true);

    // Pridani widgetu do layoutu
    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(id, 0, 0);
    widgetLayout->addWidget(idE, 0, 1);
    widgetLayout->addWidget(desc, 1, 0);
    widgetLayout->addWidget(descE, 1, 1);
    widgetLayout->addWidget(from, 2, 0);
    widgetLayout->addWidget(fromE, 2, 1);
    widgetLayout->addWidget(to, 3, 0);
    widgetLayout->addWidget(toE, 3, 1);
    widgetLayout->addWidget(weight, 4, 0);
    widgetLayout->addWidget(weightE, 4, 1);
    widgetLayout->addWidget(variable, 5, 0);
    widgetLayout->addWidget(variableE, 5, 1);

    // Nastaveni centralniho layoutu
    setLayout(widgetLayout);
}

void DiagramArcPropertiesWidget::setItem(DiagramArc *selectedItemIn)
{
    // Nastaveni vybraneho prvku a zobrazeni vlastnosti
    selectedItem = selectedItemIn;
    showProperties();
}


void DiagramArcPropertiesWidget::showProperties()
{
    /* Nastaveni zobrazovany vlastnosti z vybraneho prvku */

    idE->setText(selectedItem->properties->getID());
    descE->setText(selectedItem->properties->getDescription());
    // PLACE -> TRANSITION
    if(selectedItem->getDirection()){
        fromE->setText(selectedItem->getPlace()->properties->id);
        toE->setText(selectedItem->getTransition()->properties->id);
    }
    else{
        fromE->setText(selectedItem->getTransition()->properties->id);
        toE->setText(selectedItem->getPlace()->properties->id);
    }
    weightE->setText(QString("%1").arg(selectedItem->getWeight()));
    variableE->setText(selectedItem->getVariable());
}

void DiagramArcPropertiesWidget::setProperties(){

    /* Nastaveni vepsanych vlastnosti vybranemu prvku */

    if(idE->text().length() > 10 || idE->text().length() < 1){
        settings->getMainWindowPointer()->statusBar()->showMessage("ID length from 1 to 10 characters");
        settings->showAlert("ID length from 1 to 10 characters");
        this->showProperties();
        return;
    }
    if(weightE->text().toInt() < 1 || weightE->text().toInt() > 1000){
        settings->getMainWindowPointer()->statusBar()->showMessage("Set weight from 1 to 1000");
        settings->showAlert("Set weight from 1 to 1000");
        this->showProperties();
        return;
    }
    if(variableE->text().length() < 1 || variableE->text().length() > 5){
        settings->getMainWindowPointer()->statusBar()->showMessage("Variable length from 1 to 5 characters");
        settings->showAlert("Variable length from 1 to 5 characters");
        this->showProperties();
        return;
    }
    selectedItem->properties->id = idE->text();
    selectedItem->properties->description = descE->text();
    // from
    // to
    selectedItem->setWeight(weightE->text().toInt());
    selectedItem->setVariable(variableE->text());

    selectedItem->prepareGeometryChangePublic();
    selectedItem->update();

}

void DiagramArcPropertiesWidget::save()
{
    // Alias
    setProperties();
}

void DiagramArcPropertiesWidget::reload()
{
    // Alias
    showProperties();
}


