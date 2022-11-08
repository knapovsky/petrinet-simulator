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
 * Definice tridy s globalnim nastaveni programu.
 *
 */

#ifndef Settings_H
#define Settings_H

#include <QtCore>
#include <QtGui>
#include <QObject>
#include <QtXml>
#include <QtNetwork/QtNetwork>
#include <QTcpSocket>
#include "pointermodes.h"

class DiagramItem;
class NetworkLayer;
class SimList;

class Settings : public QObject
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param Pocatecni stav mysi
      * @return Ukazatel na nove vytvoreny objekt
      */
    Settings(Mode modeIn);

    /**
      * Ziska aktualni mod mysi
      * @return Mod mysi
      */
    Mode getCurrentMode();

    /**
      * Vyexportuje aktualni nastaveni programu do XML souboru
      */
    void saveSettingsToXML();

    /**
      * Nacte nastaveni programu z XML souboru
      * @param path Cesta k souboru s nastavenim
      */
    bool loadSettingsFromXML(QString path);

    /**
      * Nastavni mod mysi
      * @param modeIn Mod, ktery hodlame zvolit
      */
    void setCurrentMode(Mode modeIn);

    /**
      * Nastavi ukazatel na hlavni okno programu
      * @param *mainWindowPointerIn Ukazatel na hlavni okno programu
      */
    void setMainWindowPointer(QMainWindow *mainWindowPointerIn);

    /**
      * Nastavi vybrany prvek v simulacnim okne - prvek vybran 1 globalne
      * @param Vybrany prvek - Misto, prechod, nebo hrana
      */
    void setSelectedItem(DiagramItem *selectedItemIn);

    /**
      * Prekresli prvek simulacni sceny
      * @param *item Prvek, ktery ma byt prekreslen
      */
    void updateItem(DiagramItem *item);

    /**
      * Nacte puvodni nastaveni programu
      */
    void loadDefaultSettings();

    /**
      * Zobrazi okno s varovanim
      * @param message Varovani, ktere si prejeme zobrazit
      */
    void showAlert(QString message);

    /**
      * Zobrazi okno s informaci
      * @param message Zprava, ktera ma byt zobrazena
      */
    void showInfo(QString message);

    /**
      * Ziska ukazatel na hlavni okno programu
      */
    QMainWindow *getMainWindowPointer();

    /**
      * Nastavi konfiguracni soubor
      * @param configFileIn Konfiguracni soubor
      */
    void setConfigFile(QString configFileIn);

    /**
      * Aktualni mod programu
      */
    Mode currentMode;

    /**
      * Aktualne vybrany prvek
      */
    DiagramItem *selectedItem;

    /**
      * Prvek, ktery byl nakopirovan
      */
    DiagramItem *copiedItem;

    /**
      * Ukazatel na hlavni okno
      */
    QMainWindow *mainWindowPointer;

    /**
      * Pero, ktery se vykresluje vybrany objekt
      */
    QPen selectedPen;

    /**
      * Pero, ktery se vykresluje objekt, ktery nebyl vybran
      */
    QPen notSelectedPen;

    /**
      * Seznam barev, kterymi muzeme vykreslit obsah prvku
      */
    QList<QColor> colors;

    /**
      * Jmena barev
      */
    QStringList names;

    /**
      * Barva, ktera se pouzije pri vytvoreni mista ci prechodu
      */
    QColor defaultColor;

    /**
      * Barva, ktera se pouzije pri vytvoreni nove hrany
      */
    QColor defaultArcColor;

    /**
      * Retezec, ktery je pouzit pro pojmenovani noveho mista
      */
    QString defaultPlaceName;

    /**
      * Retezec, ktery je pouzit pro pojmenovani noveho prechodu
      */
    QString defaultTransitionName;

    /**
      * Retezec, ktery je pouzit pro pojmenovani nove hrany
      */
    QString defaultArcName;

    /**
      * Vaha nove vytvorene hrany
      */
    qint32 defaultWeight;

    /**
      * Vrstva, ve ktere je vykreslovana nova hrana
      */
    int defaultArcIndex;

    /**
      * Sirka sceny
      */
    qreal sceneWidth;

    /**
      * Vyska sceny
      */
    qreal sceneHeight;

    /**
      * Pozadi sceny
      */
    QBrush pattern;

    /**
      * Validace vahy
      */
    QRegExp *weightValidator;

    /**
      * Validace ID
      */
    QRegExp *idValidator;

    /**
      * Validace rozsahu
      */
    QRegExp *dimensionValidator;

    /**
      * Validace urovne zobrazeni
      */
    QRegExp *indexValidator;

    /**
      * Uzivatelske jmeno
      */
    QString username;

    /**
      * Heslo
      */
    QString password;

    /**
      * IP adresa
      */
    QString ip;

    /**
      * Port
      */
    QString port;

    /**
      * Ukazatel na objekt sitove vrstrvy
      */
    NetworkLayer *net;

    /**
      * Retezec nastaveni programu
      */
    QString configFile;

    /**
      * Ukazatel na seznamu simulaci
      */
    SimList *simList;

    /**
      * Obsah nove vytvorene simulace
      */
    QString defaultNewSimulation;

    /**
      * Timeout zpravy zobrazene ve status baru
      */
    int statusBarTimeout;

    /**
      * Urcuje zda zobrazovat vlastnosti prvku sceny ci nikoliv
      */
    bool displayProperties;

signals:

    /**
      * Vyslan pri vyberu prvku ve scene
      * @param *item Ukazatel na vybrany prvek sceny
      */
    void itemSelected(DiagramItem *item);
};

#endif // Settings_H
