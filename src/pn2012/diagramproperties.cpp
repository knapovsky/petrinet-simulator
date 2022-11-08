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
 * Implementace widgetu, ktery zobrazuje vlastnosti vybraneho prvku.
 *
 */

#include "diagramproperties.h"

DiagramProperties::DiagramProperties(SimulationData* simDataIn, Settings *&settingsIn)
{
    // Nastaveni promennych objektu
    settings = settingsIn;
    simData = simDataIn;
    itemProp = NULL;
    itemStyle = NULL;
    ok = NULL;
    cancel = NULL;
    currentlySelectedItem = NULL;
}

void DiagramProperties::setItem(DiagramItem* itemIn){

    // Pokud maji byt zobrazeny vlastnosti prvku, jehoz vlastnosti jsou jiz zobrazeni,
    // pak se nic neporovadi
    if(currentlySelectedItem == itemIn) return;

    // Nastaveni zobrazovaneho prvku
    currentlySelectedItem = itemIn;

    // Smazani widgetu
    if(itemProp != NULL || itemStyle != NULL || ok != NULL || cancel != NULL || itemIn == NULL){

        disconnect(ok, SIGNAL(clicked()), itemProp, SLOT(save()));
        disconnect(ok, SIGNAL(clicked()), itemStyle, SLOT(save()));
        disconnect(cancel, SIGNAL(clicked()), itemProp, SLOT(reload()));
        disconnect(cancel, SIGNAL(clicked()), itemStyle, SLOT(reload()));
        delete itemProp;
        delete itemStyle;
        delete ok;
        delete cancel;
        delete layout();

        itemProp = NULL;
        itemStyle = NULL;
        ok = NULL;
        cancel = NULL;

        if(itemIn == NULL){
            emit itemRemoved();
            return;
        }
    }

    // Znovuvytvoreni widgetu
    QHBoxLayout *newOne = new QHBoxLayout;
    itemProp = new DiagramItemPropertiesWidget(settings, simData);
    itemStyle = new DiagramItemStyleWidget(settings, simData);
    newOne->addWidget(itemProp);
    newOne->addWidget(itemStyle);

    QHBoxLayout *nextOne = new QHBoxLayout;
    ok = new QPushButton("OK");
    cancel = new QPushButton("Cancel");
    nextOne->addWidget(ok);
    nextOne->addWidget(cancel);

    // Propojeni signalu
    connect(ok, SIGNAL(clicked()), itemProp, SLOT(save()));
    connect(ok, SIGNAL(clicked()), itemStyle, SLOT(save()));
    connect(ok, SIGNAL(clicked()), this, SLOT(updateScene()));
    connect(cancel, SIGNAL(clicked()), itemProp, SLOT(reload()));
    connect(cancel, SIGNAL(clicked()), itemStyle, SLOT(reload()));
    /* HIDING - pri stisknuti cancel buttonu se shova widget */
    //connect(cancel, SIGNAL(clicked()), this, SLOT(hide()));

    // Vytvoreni a naplneni centralniho widgetu
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(newOne);
    centralLayout->addLayout(nextOne);

    // Nastaveni vnitrnich widgetu dle vybraneho prvku
    itemProp->setItem(itemIn);
    itemStyle->setItem(itemIn);

    // Nastaveni centralniho widgetu
    setLayout(centralLayout);
}

void DiagramProperties::updateScene(){

    // Vyslani signalu pro aktualizaci sceny
    emit itemUpdated();
}

void DiagramProperties::hide()
{
    // Pri nastaveni na NULL prvek se widget odstrani
    this->setItem(NULL);
}
