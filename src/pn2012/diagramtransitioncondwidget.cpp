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
 * Implementace widgetu zobrazujiciho podminky prechodu
 *
 */

#include "diagramtransitioncondwidget.h"
#include <QVBoxLayout>
#include <QInputDialog>

DiagramTransitionCondWidget::DiagramTransitionCondWidget(QWidget *parent)
{

    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *hbox = new QHBoxLayout(this);

    // ListWidget
    lw = new QListWidget(this);

    //
    // BUTTONS
    //
    add = new QPushButton("Add", this);
    change = new QPushButton("Change", this);
    remove = new QPushButton("Remove", this);
    removeAll = new QPushButton("Remove All", this);

    // Pridani Widgetu do layoutu
    vbox->addWidget(add);
    vbox->addWidget(change);
    vbox->addWidget(remove);
    vbox->addWidget(removeAll);

    // Pridani seznamu do layoutu
    hbox->addWidget(lw);
    hbox->addLayout(vbox);

    // Pripojeni signalu k tlacitkum
    connect(add, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(change, SIGNAL(clicked()), this, SLOT(changeItem()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(removeAll, SIGNAL(clicked()), this, SLOT(clearItems()));

    // Nastaveni centralniho layoutu
    setLayout(hbox);
}

void DiagramTransitionCondWidget::addItem(){

    bool okPressed = false;

    // Vyvolani nabidky na pridani podminky
    QString item = QInputDialog::getText(this, "Item",
           "Enter new item", QLineEdit::Normal, QString(), &okPressed, 0);
    item = item.simplified();

    // Pri pridani podminky je
    // podminka pridana primo k prechodu
    // a seznam je aktulizovany
    if (!item.isEmpty() && okPressed) {

      lw->addItem(item);
      int r = lw->count() - 1;
      lw->setCurrentRow(r);
      diagramTransition->conditions.append(item);
    }
}

void DiagramTransitionCondWidget::setItem(DiagramTransition *diagramTransitionIn){

    lw->clear();

    // Zobrazuje podminky zvoleneho prechodu
    diagramTransition = diagramTransitionIn;

    // Prochazeji se podminky prechodu a ty jsou zobrazovany
    QList<QString>::iterator iter;
    for(iter = diagramTransition->conditions.begin(); iter != diagramTransition->conditions.end(); iter++){

        lw->addItem(*iter);
    }
}

void DiagramTransitionCondWidget::changeItem()
{
    // Vybrani soucasne zvoleneho prvku
    QListWidgetItem *curitem = lw->currentItem();
    int r = lw->row(curitem);

    // Nastaveni textu nabidky na zvolenou polozku
    QString text = curitem->text();

    // Vyvolani dialogoveho okna s nabidkou
    QString item = QInputDialog::getText(this, "Item",
             "Enter new item", QLineEdit::Normal, text);
    item = item.simplified();

    // Pokud neni nabidka prazdna, aktulizuje se podminka prechodu
    if (!item.isEmpty()) {

        // Odstraneni polozky seznamu
        lw->takeItem(r);
        delete curitem;

        // Vlozeni nove polozky
        lw->insertItem(r, item);
        lw->setCurrentRow(r);

        // Nahrazeni podminky v prechodu
        diagramTransition->conditions.replace(r, item);

        // Aktualizace prechodu ve scene
        diagramTransition->update();
    }
}

void DiagramTransitionCondWidget::removeItem()
{    
    // Odstrani podminku z prechodu a seznamu podminek
    // Vybere aktualne zvolenou podminku
    QListWidgetItem *item = lw->currentItem();

    // Pokud byla nejaka podminka vybrana
    if (item) {

        // Odstrani se ze seznamu
        int r = lw->row(item);
        lw->takeItem(r);
        delete item;

        // Odstrani se z podminek v prechodu
        diagramTransition->conditions.removeAt(r);

        // Aktualizuje se prechod ve scene
        diagramTransition->update();
    }
}

void DiagramTransitionCondWidget::clearItems()
{

    // Odstrani vsechny podminky prechodu
    if (lw->count() != 0){

        // Odstraneni polozek seznamu
        lw->clear();
        QString i;

        // Odstranovani vsech podminek u prechodu
        foreach(i, diagramTransition->conditions){
            diagramTransition->conditions.removeFirst();
        }

        // Aktualizace sceny
        diagramTransition->update();
    }

}
