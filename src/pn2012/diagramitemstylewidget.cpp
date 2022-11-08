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
 * Implementace widgetu, ktery slouzi k zobrazovani nize uvedenych vlastnosti prvku.
 *
 * Barva, pozice, vyska, sirka, index
 *
 */

#include "diagramitemstylewidget.h"

DiagramItemStyleWidget::DiagramItemStyleWidget(Settings *&settingsIn, SimulationData *simulationDataIn)
{
    // Nastaveni promennych objektu
    settings = settingsIn;
    simulationData = simulationDataIn;

    // Vytvoreni vnitrnich widgetu
    createInsideWidgets();
}

void DiagramItemStyleWidget::setItem(DiagramItem *selectedItemIn)
{
    // Nastaveni vybraneho prvku a zobrazeni jeho vlastnosti
    selectedItem = selectedItemIn;
    showItemStyleProperties();
}

void DiagramItemStyleWidget::save()
{
    // Ulozeni vepsanych vlastnosti
    setItemStyleProperties();
}

void DiagramItemStyleWidget::reload()
{
    // Znovunacteni a zobrazeni vlastnosti prvku
    showItemStyleProperties();
}

void DiagramItemStyleWidget::setColor(int i)
{
    // Nastaveni barvy prvku
    selectedItem->properties->color = settings->colors.at(i);
    selectedItem->update();
}

void DiagramItemStyleWidget::showItemStyleProperties()
{
    // Zobrazeni vlastnosti prvku dle je typu
    if(selectedItem->type == Arc){
        xE->setText(tr("NA"));
        yE->setText(tr("NA"));
        heightE->setDisabled(true);
        widthE->setDisabled(true);
        heightE->setText(tr("NA"));
        widthE->setText(tr("NA"));
        indexE->setText(QString("%1").arg(selectedItem->properties->getIndex()));
    }
    else{
        heightE->setDisabled(false);
        widthE->setDisabled(false);
        //valuesString.append(QString("%1").arg(*i));
        xE->setText(QString("%1").arg(selectedItem->properties->getX() + selectedItem->properties->width/2));
        yE->setText(QString("%1").arg(selectedItem->properties->getY() + selectedItem->properties->height/2));
        heightE->setText(QString("%1").arg(selectedItem->properties->getHeight()));
        widthE->setText(QString("%1").arg(selectedItem->properties->getWidth()));
        indexE->setText(QString("%1").arg(selectedItem->properties->getIndex()));
        // TODO
        //colorE->setText(selectedItem->properties->getColor());
    }
}

void DiagramItemStyleWidget::setItemStyleProperties(){

    // Ulozeni vepsanych vlastnosti prvku

    if((heightE->text().toFloat() < 20 || heightE->text().toFloat() > 250) && selectedItem->type != Arc){
        settings->getMainWindowPointer()->statusBar()->showMessage("Please enter height from 20 to 250");
        settings->showAlert("Please enter height from 20 to 250");
        this->showItemStyleProperties();
        return;
    }
    if((widthE->text().toFloat() < 20 || widthE->text().toFloat() > 250) && selectedItem->type != Arc){
        settings->getMainWindowPointer()->statusBar()->showMessage("Please enter width from 20 to 250");
        settings->showAlert("Please enter width from 20 to 250");
        this->showItemStyleProperties();
        return;
    }
    if(indexE->text().toInt() < -100 || widthE->text().toFloat() > 100){
        settings->getMainWindowPointer()->statusBar()->showMessage("Enter index from -100 to 100");
        settings->showAlert("Enter index from -100 to 100");
        this->showItemStyleProperties();
        return;
    }

    if(selectedItem->type != Arc){
        selectedItem->properties->height = heightE->text().toFloat();
        selectedItem->properties->width = widthE->text().toFloat();
    }
    selectedItem->properties->index = indexE->text().toInt();
    // TODO
    //selectedItem->properties->color = colorE->text();
    selectedItem->prepareGeometryChangePublic();
    selectedItem->update();

}

void DiagramItemStyleWidget::createInsideWidgets(){

    //
    // LABELY
    //
    QGridLayout *widgetLayout = new QGridLayout;
    //QLabel *id = new QLabel("ID:", this);
    QLabel *x = new QLabel("X Position:", this);
    QLabel *y = new QLabel("Y Position:", this);
    QLabel *height = new QLabel("Height:", this);
    QLabel *width = new QLabel("Width:", this);
    QLabel *index = new QLabel("Overlay index:", this);
    QLabel *color = new QLabel("Item Color:", this);

    /* Color Menu */
    QComboBox *comboBox = new QComboBox(this);
    QString colorName;
    QIcon icon;
    foreach(colorName, settings->names){
        comboBox->addItem(icon, colorName);
    }
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setColor(int)));

    //
    // LINE EDITS
    //
    xE = new QLineEdit(this);
    yE = new QLineEdit(this);
    heightE = new QLineEdit(this);
    //heightE->setValidator(new QRegExpValidator(*settings->dimensionValidator, this));
   // heightE->setInputMask("D00");
    widthE = new QLineEdit(this);
    //widthE->setValidator(new QRegExpValidator(*settings->dimensionValidator, this));
    //widthE->setInputMask("D00");
    indexE = new QLineEdit(this);
    //indexE->setValidator(new QRegExpValidator(*settings->indexValidator, this));
    //indexE->setInputMask("000");
    //colorE = new QLineEdit(this);

    xE->setDisabled(true);
    yE->setDisabled(true);

    // Pridani widgetu do layoutu
    widgetLayout->addWidget(x, 0, 0);
    widgetLayout->addWidget(xE, 0, 1);
    widgetLayout->addWidget(y, 1, 0);
    widgetLayout->addWidget(yE, 1, 1);
    widgetLayout->addWidget(height, 2, 0);
    widgetLayout->addWidget(heightE, 2, 1);
    widgetLayout->addWidget(width, 3, 0);
    widgetLayout->addWidget(widthE, 3, 1);
    widgetLayout->addWidget(index, 4, 0);
    widgetLayout->addWidget(indexE, 4, 1);
    widgetLayout->addWidget(color, 5, 0);
    widgetLayout->addWidget(comboBox, 5, 1);

    // Nastaveni centralniho layoutu
    setLayout(widgetLayout);
}


