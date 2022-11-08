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
 * Implementace tridy zobrazujici prijaty seznam siti na serveru.
 * Umoznuje uzivateli prochazet, vyhledavat a otevrit zvoleny soubor.
 *
 */

#include "networkbrowser.h"

NetworkBrowser::NetworkBrowser(Settings *settingsIn)
{
    settings = settingsIn;

    /* Nastaveni prazdneho obsahu */
    netList.setContent(QString(""));

    // Vytvoreni vnitrnich widgetu
    searchButton = new QPushButton("Search", this);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPressed()));
    okButton = new QPushButton("OK", this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
    searchLine = new QLineEdit(this);
    treeBrowser = new QTreeWidget(this);
    connect(treeBrowser, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(showHistory(QTreeWidgetItem*, int)));

    itemHistoryBrowser = new QTreeWidget(this);

    // Checkboxy
    checkName = new QCheckBox("Name", this);
    checkName->setChecked(true);
    checkVersion = new QCheckBox("Version", this);
    checkVersion->setChecked(false);
    checkAuthor = new QCheckBox("Author", this);
    checkAuthor->setChecked(false);
    checkDate = new QCheckBox("Date", this);
    checkDate->setChecked(false);

    // Layouty
    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(checkName);
    checkBoxLayout->addWidget(checkVersion);
    checkBoxLayout->addWidget(checkAuthor);
    checkBoxLayout->addWidget(checkDate);

    /* Name, Version, Author, Date */
    treeBrowser->setColumnCount(4);

    // Nastaveni nazvu jednotlivych sloupcu
    treeBrowser->setHeaderLabels(QStringList() << "Name" << "Version" << "Author" << "Date");

    // Nastaveni nazvu jednotlivych sloupcu historie
    itemHistoryBrowser->setColumnCount(3);
    itemHistoryBrowser->setHeaderLabels(QStringList() << "Date" << "Time" << "Login");
    //itemHistoryBrowser->setDisabled(true);

    // Pridani widgetu do layoutu
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLine);
    searchLayout->addWidget(searchButton);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QHBoxLayout *browserLayout = new QHBoxLayout;
    browserLayout->addWidget(treeBrowser);
    browserLayout->addWidget(itemHistoryBrowser);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addLayout(searchLayout);
    widgetLayout->addLayout(checkBoxLayout);
    widgetLayout->addLayout(browserLayout);
    widgetLayout->addLayout(buttonLayout);

    // Nastaveni hlavniho layoutu
    this->setLayout(widgetLayout);
    this->setBaseSize(300, 500);
}

void NetworkBrowser::setBrowser(QDomDocument netListIn)
{
    if(!this->itemList.isEmpty()){
        this->itemList.clear();
    }

    /* Vycisteni browseru */
    treeBrowser->clear();

    this->netList = netListIn;

    // Reference na prvniho potomka dokumentu;
    QDomElement root = netListIn.firstChildElement();

    QDomNodeList items = root.elementsByTagName("net");

    for(int i = 0; i < items.count(); i++){

        QDomNode itemNode = items.at(i);

       //convert to element
       if(itemNode.isElement())
       {
            QDomElement elem = itemNode.toElement();

            QString name = elem.attribute("name");
            QString version = elem.attribute("version");
            QString author = elem.attribute("author");
            QString date = elem.attribute("date");

            QTreeWidgetItem *item = new QTreeWidgetItem(treeBrowser);
            item->setFirstColumnSpanned(false);

            item->setText(0, name);
            item->setText(1, version);
            item->setText(2, author);
            item->setText(3, date);

            // Ziskani simulacni historie
            QDomNodeList historyItems = elem.elementsByTagName("history");
            QList<QList<QString> > historyList;

            for(int i = 0; i < historyItems.count(); i++){

                QDomNode historyNode = historyItems.at(i);

                if(historyNode.isElement()){

                    QDomElement historyEl = historyNode.toElement();

                    QString login = historyEl.attribute("by");
                    QString time = historyEl.attribute("time");
                    QString date = historyEl.attribute("date");

                    QList<QString> historyAttributes;
                    historyAttributes.append(login);
                    historyAttributes.append(time);
                    historyAttributes.append(date);

                    // Pripojeni polozky historie k ostatnim polozkam
                    historyList.append(historyAttributes);

                    //qDebug() << "history: " << historyAttributes;
                }
            }

            this->itemList.append(item);

            // Pripojeni historii prvku k ostatnim
            this->historiesOfItems.append(historyList);

            // Namapovani prvku na jeho historii
            this->itemToHistoryMap.insert(item, this->historiesOfItems.length() - 1);
        }
    }

    /* Serazeni podle data */
    treeBrowser->sortItems(4, Qt::DescendingOrder);
    /* Serazeni podle verzi */
    treeBrowser->sortItems(2, Qt::DescendingOrder);

    //qDebug() << "histories: " << historiesOfItems;
}

void NetworkBrowser::okPressed()
{
    if(treeBrowser->currentItem() == NULL){
        //qDebug() << "Item not selected";
        this->close();
    }
    else{

        QTreeWidgetItem *selectedItem = treeBrowser->currentItem();
        QString netName = selectedItem->text(0);
        QString version = selectedItem->text(1);

        this->settings->net->sendGetNet(netName, version);
        this->close();
    }

}

void NetworkBrowser::cancelPressed()
{
    this->close();
}

void NetworkBrowser::searchPressed()
{
    /* Zviditelneni vsech prvku */
    int i = 0;
    for(i = 0; i < treeBrowser->topLevelItemCount(); i++){
        treeBrowser->topLevelItem(i)->setHidden(false);
    }

    if(this->netList.toString().isEmpty()){
        return;
    }
    if(searchLine->text().isEmpty()){
        return;
    }
    else{

        // Najde prvky podle zadaneho textu
        QList<QTreeWidgetItem *> foundItemsName = treeBrowser->findItems(searchLine->text(), Qt::MatchContains, 0);
        QList<QTreeWidgetItem *> foundItemsVersion = treeBrowser->findItems(searchLine->text(), Qt::MatchContains, 1);
        QList<QTreeWidgetItem *> foundItemsAuthor = treeBrowser->findItems(searchLine->text(), Qt::MatchContains, 2);
        QList<QTreeWidgetItem *> foundItemsDate = treeBrowser->findItems(searchLine->text(), Qt::MatchContains, 3);

        // Sestroji vysledek vyhledavani podle pozadovanych poli
        QList<QTreeWidgetItem *> result;
        if(checkName->isChecked()){
            result.append(foundItemsName);
        }
        if(checkVersion->isChecked()){
            result.append(foundItemsVersion);
        }
        if(checkAuthor->isChecked()){
            result.append(foundItemsAuthor);
        }
        if(checkDate->isChecked()){
            result.append(foundItemsDate);
        }


        QList<QTreeWidgetItem *> finalResult;

        /* Procisteni vysledku vyhledavani */
        QList<QTreeWidgetItem *>::iterator iter;
        for(iter = itemList.begin(); iter != itemList.end(); iter++){
            QList<QTreeWidgetItem *>::iterator iter2;
            for(iter2 = result.begin(); iter2 != result.end(); iter2++){
                if(*iter == *iter2){
                    finalResult.append(*iter);
                    break;
                }
            }
        }

        /* Najdu prvky, ktere se maji schovat */
        QList<QTreeWidgetItem *> inverseFinalResult;
        QList<QTreeWidgetItem *>::iterator itemIter;
        QList<QTreeWidgetItem *>::iterator finalResIter;
        bool found = false;
        int i = 0;
        for(i = 0; i < treeBrowser->topLevelItemCount(); i++){

            found = false;
            for(finalResIter = finalResult.begin(); finalResIter != finalResult.end(); finalResIter++){

                /* Prvek, ktery byl nalezen pres search */
                if((*finalResIter) == treeBrowser->topLevelItem(i)){

                    found = true;
                    break;
                }
            }
            if(!found){

                inverseFinalResult.append(treeBrowser->topLevelItem(i));
            }
        }

        /* Schovani prvku, ktere nebyli nalezeny */
        QList<QTreeWidgetItem *>::iterator hideIter;
        for(hideIter = inverseFinalResult.begin(); hideIter != inverseFinalResult.end(); hideIter++){

            treeBrowser->setItemHidden(*hideIter, true);
        }
    }
}

void NetworkBrowser::showHistory(QTreeWidgetItem *item, int index){


    itemHistoryBrowser->clear();

    //qDebug()<< "Index: " << this->itemToHistoryMap.value(item);

    QList<QList<QString > > itemHistories = this->historiesOfItems.at(this->itemToHistoryMap.value(item));
    //qDebug() << "ItemHistories: " << itemHistories;

    // Pruchod historiemi prvku
    QList<QList<QString > >::iterator itemHistoriesIter;
    for(itemHistoriesIter = itemHistories.begin(); itemHistoriesIter != itemHistories.end(); itemHistoriesIter++){

        QList<QString> itemHistory = (*itemHistoriesIter);

        //qDebug() << "itemHistory: " << itemHistory;

        QString login = itemHistory.at(0);
        QString date = itemHistory.at(1);
        QString time = itemHistory.at(2);

        QTreeWidgetItem *item = new QTreeWidgetItem(itemHistoryBrowser);
        item->setFirstColumnSpanned(false);

        item->setText(0, date);
        item->setText(1, time);
        item->setText(2, login);

        this->historyList.append(item);
    }
}
