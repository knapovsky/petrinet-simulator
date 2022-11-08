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
 * Implementace widgetu, ktery zobrazuje operace pridruzene k prechodu a
 * a umoznuje je editovat
 *
 */

#include "diagramtransitionopwidget.h"
#include <QVBoxLayout>
#include <QInputDialog>

DiagramTransitionOpWidget::DiagramTransitionOpWidget(QWidget *parent)
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

void DiagramTransitionOpWidget::addItem(){

    bool okPressed = false;

    // Vyvolani nabidky na pridani operace
    QString item = QInputDialog::getText(this, "Item",
           "Enter new item", QLineEdit::Normal, QString(), &okPressed, 0);
    item = item.simplified();

    // Pri pridani operace je
    // operace pridana primo k prechodu
    // a seznam je aktulizovany
    if (!item.isEmpty() && okPressed) {

        // Pridani operace do seznamu
        lw->addItem(item);
        int r = lw->count() - 1;
        lw->setCurrentRow(r);

        // Pridani operace k prechodu
        diagramTransition->operations.append(item);
    }
}

void DiagramTransitionOpWidget::setItem(DiagramTransition *diagramTransitionIn){

    lw->clear();

    // Zobrazuje operace zvoleneho prechodu
    diagramTransition = diagramTransitionIn;

    // Prochazeji se operace prechodu a ty jsou zobrazeny
    QList<QString>::iterator iter;
    for(iter = diagramTransition->operations.begin(); iter != diagramTransition->operations.end(); iter++){

        lw->addItem(*iter);
    }
}

void DiagramTransitionOpWidget::changeItem(){

    // Nastaveni aktualne zvolene polozky seznamu
    QListWidgetItem *curitem = lw->currentItem();
    int r = lw->row(curitem);

    // Nastaveni zobrazovaneho textu
    QString text = curitem->text();
    QString item = QInputDialog::getText(this, "Item",
             "Enter new item", QLineEdit::Normal, text);
    item = item.simplified();

    // Pokud nebyl text v dialogu prazdny, zmeni se operace
    if (!item.isEmpty()) {

        // Odebere se operace
        lw->takeItem(r);
        delete curitem;

        // Prida se nova operace
        lw->insertItem(r, item);
        lw->setCurrentRow(r);

        // Zmeni se operace u prechodu
        diagramTransition->operations.replace(r, item);

        // Prechod ve scene se aktualizuje
        diagramTransition->update();
    }
}

void DiagramTransitionOpWidget::removeItem()
{
    // Odstranuje polozku seznamu
    QListWidgetItem *item = lw->currentItem();

    // Pokud byla polozka vybrana
    if (item) {

        // Odstraneni ze seznamu
        int r = lw->row(item);
        lw->takeItem(r);
        delete item;

        // Odstraneni operace z prechodu
        diagramTransition->operations.removeAt(r);

        // Aktualizace prechodu ve scene
        diagramTransition->update();
    }
}

void DiagramTransitionOpWidget::clearItems()
{
    // Odstrani vsechny polozky seznamu
    if (lw->count() != 0){

        // Vycisteni seznamu
        lw->clear();

        // Pruchod operacemi prechodu a jejich odstraneni
        QString i;
        foreach(i, diagramTransition->operations){
            diagramTransition->operations.removeFirst();
        }

        // Aktualizace prechodu ve scene
        diagramTransition->update();
    }

}
