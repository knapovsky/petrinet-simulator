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
 * Implementace tridy, ktera reprezentuje custom Qt widget, pres ktery se nastavuji
 * vlastnosti simulace - Autor, Popis, ...
 *
 */

#include "simulationpropertieswidget.h"

SimulationPropertiesWidget::SimulationPropertiesWidget(SimulationData *simDataIn, Settings *settingsIn, QDialog *dialogIn)
{

    this->setParent(settingsIn->mainWindowPointer);

    // Inicializace
    settings = settingsIn;
    dialog = dialogIn;
    simData = simDataIn;

    /* Hlavni layout */
    QGridLayout *widgetLayout = new QGridLayout;

    //
    // Labely
    //
    QLabel *nameL = new QLabel("Name", this);
    QLabel *authorL = new QLabel("Author", this);
    QLabel *versionL = new QLabel("Version", this);
    QLabel *descriptionL = new QLabel("Description", this);
    QLabel *dateL = new QLabel("Date", this);

    //
    // Line edits
    //
    nameE = new QLineEdit(this);
    authorE = new QLineEdit(this);
    versionE = new QLineEdit(this);
    versionE->setDisabled(true);
    //versionE->setInputMask("0.0;");
    descriptionE = new QTextEdit(this);
    dateE = new QLineEdit(this);
    dateE->setDisabled(true);

    nameL->setBuddy(nameE);
    authorL->setBuddy(authorE);
    versionL->setBuddy(versionE);
    descriptionL->setBuddy(descriptionE);
    dateL->setBuddy(dateE);

    // Pridani prvku do layoutu
    widgetLayout->addWidget(nameL, 0, 0);
    widgetLayout->addWidget(nameE, 0, 1);
    widgetLayout->addWidget(authorL, 1, 0);
    widgetLayout->addWidget(authorE, 1, 1);
    widgetLayout->addWidget(versionL, 2, 0);
    widgetLayout->addWidget(versionE, 2, 1);
    widgetLayout->addWidget(dateL, 3, 0);
    widgetLayout->addWidget(dateE, 3, 1);
    widgetLayout->addWidget(descriptionL, 4, 0);
    widgetLayout->addWidget(descriptionE, 4, 1);

    // Zobrazeni hodnot do prvku widgetu
    showProperties();

    // Nastaveni hlavniho layoutu widgetu
    setLayout(widgetLayout);
}

void SimulationPropertiesWidget::showProperties(){

    nameE->setText(simData->name);
    authorE->setText(simData->author);
    versionE->setText(simData->version);
    dateE->setText(simData->date);
    descriptionE->setText(simData->description);

}

void SimulationPropertiesWidget::setProperties(){

    simData->name = nameE->text();
    simData->author = authorE->text();
    simData->description = descriptionE->toPlainText();

    dialog->accept();

    //settings->getMainWindowPointer()->
    settings->getMainWindowPointer()->statusBar()->showMessage("Properties has been saved");

}


