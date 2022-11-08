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
 * Implementace custom Qt widgetu, ktery zobrazuje ohodnoceni mista v
 * seznamu a umoznuje jejich pridavani a editaci.
 *
 */

#include "listwidget.h"
#include <QVBoxLayout>
#include <QInputDialog>


ListWidget::ListWidget(QWidget *parent)
    : QWidget(parent)
{

    //
    // Layouty
    //
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout(this);

    // List Widget
    lw = new QListWidget(this);

    //
    // BUTTONS
    //
    add = new QPushButton("Add", this);
    remove = new QPushButton("Remove", this);
    removeAll = new QPushButton("Remove All", this);

    // Pridani widgetu do layoutu
    vbox->addWidget(add);
    vbox->addWidget(remove);
    vbox->addWidget(removeAll);

    hbox->addWidget(lw);
    hbox->addLayout(vbox);

    // Napojeni signalu od tlacitek
    connect(add, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(removeAll, SIGNAL(clicked()), this, SLOT(clearItems()));

    // Nastaveni hlavniho layoutu
    setLayout(hbox);
}

void ListWidget::addItem()
{
    // Prida novou polozku do list widgetu - vyvolava nabidku

    qint32 item = INT_MIN;
    bool okPressed = false;
    item = QInputDialog::getInt(this, "Item",
         "Enter new item", 0, -2147483647, 2147483647, 1, &okPressed, 0);
    //item = item.simplified();

    // Pokud nebyl obsah nabidky prazdny a stisknuto tlacitko OK
    if (okPressed) {

        // Pridani plozky
        lw->addItem(QString("%1").arg(item));
        int r = lw->count() - 1;

        // Nastaveni polozky jako aktualni
        lw->setCurrentRow(r);

        // Pridani hodnoty k danemu mistu
        diagramPlace->values.append(item);
    }
}

void ListWidget::setItem(DiagramPlace *diagramPlaceIn){

    diagramPlace = diagramPlaceIn;
    QList<int>::iterator iter;
    for(iter = diagramPlace->values.begin(); iter != diagramPlace->values.end(); iter++){
        lw->addItem(QString("%1").arg(*iter));
    }
}

/*
void ListWidget::renameItem()
{
  QListWidgetItem *curitem = lw->currentItem();
  int r = lw->row(curitem);
  QString text = curitem->text();
  QString item = QInputDialog::getText(this, "Item",
             "Enter new item", QLineEdit::Normal, text);
  item = item.simplified();
  if (!item.isEmpty()) {
    lw->takeItem(r);
    delete curitem;
    lw->insertItem(r, item);
    lw->setCurrentRow(r);
  }
}*/

void ListWidget::removeItem()
{
  QListWidgetItem *item = lw->currentItem();

  if (item) {
    int r = lw->row(item);
    lw->takeItem(r);
    delete item;
    diagramPlace->values.removeAt(r);
    diagramPlace->update();
  }
}

void ListWidget::clearItems()
{
  if (lw->count() != 0){
    lw->clear();
    qint32 i;
    foreach(i, diagramPlace->values){
        diagramPlace->values.removeFirst();
    }
    diagramPlace->update();
  }

}
