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
 * Hlavickovy soubor s definici tridy zobrazujici prijaty seznam siti na serveru.
 * Umoznuje uzivateli prochazet, vyhledavat a otevrit zvoleny soubor.
 *
 */

#ifndef NETWORKBROWSER_H
#define NETWORKBROWSER_H

//#include <QtCore>
//#include <QtGui>
//#include <QListView>
//#include <QtXml>
#include <settings.h>
#include <networklayer.h>

class NetworkBrowser : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *settingsIn Ukazatel na globalni nastaveni programu
      * @return Ukazatel na nove vytvoreny objekt
      */
    NetworkBrowser(Settings* settingsIn);

    /**
      * Zobrazi ve widgetu obsah prijateho XML souboru
      * @param netListIn Prijaty XML soubor
      */
    void setBrowser(QDomDocument netListIn);

public slots:

    /**
      * Volan pri stisknuti OK
      */
    void okPressed();

    /**
      * Stisk tlacitka cancel
      */
    void cancelPressed();

    /**
      * Stisk tlacitka pro vyhledavani
      */
    void searchPressed();

    /**
      * Pri stisku tlacitka na polozku seznamu vypise jeho simulacni historii
      * @param *item Ukazatel na aktualne vybranou polozku seznamu siti
      * @param index Poradi aktualne zvolene polozky v seznamu siti
      */
    void showHistory(QTreeWidgetItem *item, int index);

private:

    /**
      * Ukazatel na Search Button
      */
    QPushButton *searchButton;

    /**
      * Ukazatel na OK Button
      */
    QPushButton *okButton;

    /**
      * Ukazatel na Cancel Button
      */
    QPushButton *cancelButton;

    /**
      * Ukazatel na vstup vyhledavani
      */
    QLineEdit *searchLine;

    /**
      * Ukazatel na seznam zobrazujici polozky XML
      */
    QTreeWidget *treeBrowser;

    /**
      * Ukazatel na seznam zobrazujici historii
      * simulaci zvolene polozky
      */
    QTreeWidget *itemHistoryBrowser;

    /**
      * Ukazatel na Name CheckBox
      */
    QCheckBox *checkName;

    /**
      * Ukazatel na Version CheckBox
      */
    QCheckBox *checkVersion;

    /**
      * Ukazatel na Author CheckBox
      */
    QCheckBox *checkAuthor;

    /**
      * Ukazatel na Date CheckBox
      */
    QCheckBox *checkDate;

    /**
      * Ukazatel na objekt globalniho nastaveni programu
      */
    Settings *settings;

    /**
      * Zobrazovany XML soubor
      */
    QDomDocument netList;

    /**
      * Seznam zobrazenych prvku
      */
    QList<QTreeWidgetItem* > itemList;

    /**
      * Seznam historii jednotlivych prvku seznamu siti
      */
    QList<QList<QList<QString> > > historiesOfItems;

    /**
      * Seznam aktualne zobrazenych historii prvku ze seznamu siti
      */
    QList<QTreeWidgetItem* > historyList;

    /**
      * Mapovani ukazatele zobrazeneho prvku v seznamu siti
      * na jeho historii
      */
    QMap<QTreeWidgetItem*, int> itemToHistoryMap;
};

#endif // NETWORKBROWSER_H
