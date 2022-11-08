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
 * Tato trida uchovava vsechna simulacni data u umoznuje s nimi pracovat
 *
 */

#ifndef SIMULATIONSETTINGS_H
#define SIMULATIONSETTINGS_H

//#include <QtCore>
//#include <QtGui>
//#include <QtXml>
#include "diagramplace.h"
#include "diagramtransition.h"
#include "diagramarc.h"

class Settings;
class Simulation;
class DiagramProperties;
class DiagramItem;


class SimulationData : public QObject
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param fileNameIn Nazev simulace
      * @param simFileIn Vstupni soubor s popisem simulace
      * @param *&settingsIn Globalni nastaveni programu
      * @param *simulationIn Ukazatel na simulaci, ktera bude tento objekt vlastnit
      * @param *propertiesIn Ukazatel na objekt, ktery zobrazuje vlastnosti vybraneho prvku simulace
      * @return Ukazatel na nove vytvoreny objekt
      */
    SimulationData(QString fileNameIn,
                   QString simFileIn,
                   Settings *&settingsIn,
                   Simulation *simulationIn,
                   DiagramProperties *propertiesIn
                   );

    /**
      * Vrati retezec se jmenem simulace
      * @return Retezec se jmenem simulace
      */
    QString getFilename();

    /**
      * Vygeneruje a ulozi soubor popisujici simulaci na predane umisteni
      * @param Umisteni vygenerovaneho souboru
      */
    void generateXML(QString fileLocation);

    /**
      * Nacte vstupni XML soubor a nastavi dle nej vnitrni promenne objektu
      */
    void parseXML();

    /**
      * Nacte vysledek simulace od serveru a upravi dle nej vnitrni promenne objektu
      * @param update Vysledek simulace od serveru
      */
    void parseUpdate(QString update);

    /**
      * Zkopiruje vybrany prvek sceny
      */
    void copySelectedItem();

    /**
      * Odstrani vybrany prvek sceny
      */
    void removeSelectedItem();

    /**
      * Odstrani vsechny hrany, ktere jsou pripojeny
      * k mistu, nebo prechodu s danym id
      * @param id ID prechodu, nebo mista
      */
    void removeArcContaining(QString id);

    /**
      * Indickace toho, zda seznam typu INT obsahuje danou polozku
      * @param list Seznam polozek
      * @param item Vyhledavana polozka
      * @return Indikace, zda je polozka v seznamu obsazena, ci nikoliv
      */
    bool listContains(QList<int> list, int item);

    /**
      * Vyjme vybranou polozku ve scene
      */
    void cutSelectedItem();

    /**
      * Vlozi zkopirovanou, nebo vynatou polozku do sceny
      */
    void pasteItem();

    /**
      * Aktualizuje datum simulacnich dat na aktualni
      */
    void updateNetDate();

    /**
      * Zvysi verzi site o 0.1
      */
    void updateNetVersion();

    /**
      * Vyprazdni aktualni simualcni data a nahraje soubor znovu
      */
    void reloadSimulation();

    /**
      * Zjisti, zda se simulacni prvek nachazi v teto simualaci
      * @param *item Vyhledavany prvek
      * @return Indikace, zda je prvek obsazen v teto simulaci
      */
    bool isItemHere(DiagramItem *item);

    /**
      * Zkontroluje, zda jsou v simulacnim souboru obsazeny
      * vsechny nalezitosti, aby bylo mozne simulaci spravne
      * vykonat.
      * @return Indikace toho, zda je vse obsazeno, ci nikoliv
      */
    bool checkSyntaxBeforeSimulation();

    /**
      * Ziska unikatni jmeno pro nove vytvarene misto
      * @return Unikatni jmeno mista
      */
    QString getNewPlaceName();

    /**
      * Ziska unikatni jmeno pro nove vytvareny prechod
      * @return Unikatni jmeno prechodu
      */
    QString getNewTransitionName();

    /**
      * Ziska unikatni jmeno pro nove vytvarenou hranu
      * @return Unikatni jmeno hrany
      */
    QString getNewArcName();

    /**
      * Vrati simulacni soubor
      * @return Simulacni soubor
      */
    QDomDocument getNet();

    /**
      * Ulozi momentalni simulacni data
      * do simFile pro zpetne nahrani
      */
    void saveCurrentNet();


private:

    /**
      * Dle vstupniho simulacniho souboru nacte obsah,
      * ktery popisuje simulacni mista
      * @param root DOM Element, ktery mista obsahuje
      */
    void parsePlaceElements(QDomElement root);

    /**
      * Dle vstupniho simulacniho souboru nacte obsah,
      * ktery popisuje simulacni prechody
      * @param root DOM Element, ktery prechody obsahuje
      */
    void parseTransitionElements(QDomElement root);

    /**
      * Dle vstupniho simulacniho souboru nacte obsah,
      * ktery popisuje simulacni hrany
      * @param root DOM Element, ktery hrany obsahuje
      */
    void parseArcElements(QDomElement root);

    /**
      * Dle vstupniho simulacniho souboru nacte obsah,
      * ktery obsahuje popis simulace - jmeno, datum, verze, ...
      * @param root DOM Element, ktery popis obsahuje
      */
    void parseHead(QDomElement root);

    /**
      * Vrati retezec s nazvem barvy, ktera je danem indexu globalniho nastaveni
      * @param colorIn Index barvy
      * @return Retezec s nazvem barvy
      */
    QString colorDecoder(QColor colorIn);

signals:

    /**
      * Chyba nacitani souboru
      */
    void parsingError();

    /**
      * Odstrani hrany, ktere jsou urceny k odstraneni
      * toRemoveDiagramArcs
      */
    void removeArcs();

    /**
      * Odstrani prvek, ktery je urcen k odstraneni
      * - toRemoveItem
      */
    void removeItem();

    /**
      * Odstrani prvek, ktery je urcen k vlozeni
      * - toInsertItem
      */
    void insertItem();

public:

    /**
      * Jmeno autora simulacniho souboru
      */
    QString author;

    /**
      * Datum simulacnich dat
      */
    QString date;

    /**
      * Nazev simulace
      */
    QString name;

    /**
      * Popis simulace
      */
    QString description;

    /**
      * Verze simulace
      */
    QString version;

    /**
      * Misto urceno ke kopirovani
      */
    DiagramPlace *copyPlace;

    /**
      * Prechod urceny ke kopirovani
      */
    DiagramTransition *copyTransition;

    /**
      * Seznam mist
      */
    QList<DiagramPlace*> diagramPlaces;

    /**
      * Seznam prechodu
      */
    QList<DiagramTransition*> diagramTransitions;

    /**
      * Seznam propojeni
      */
    QList<DiagramArc*> diagramArcs;

    /**
      * Seznam hran k odstraneni ze sceny
      */
    QList<DiagramArc*> toRemoveDiagramArcs;

    /**
      * Prvek urceny k odstraneni
      */
    DiagramItem* toRemoveItem;

    /**
      * Prvek urceny k vlozeni
      */
    DiagramItem* toInsertItem;

    /**
      * Nazev otevreneho souboru
      */
    QString fileName;

    /**
      * Obsah simulacniho souboru
      */
    QString simFile;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na simulaci, jejiz obsahem jsou tato data
      */
    Simulation *simulation;

    /**
      * Ukazatel na widget zobrazujici vlastnosti vybraneho prvku sceny
      */
    DiagramProperties *properties;

    /**
      * Pocitadlo prechodu - slouzi k vytvoreni unikatniho jmena prechodu
      */
    int transitionCounter;

    /**
      * Pocitadlo mist - slouzi k vytvoreni unikatniho jmena mista
      */
    int placeCounter;

    /**
      * Pocitadlo hran - slouzi k vytvoreni unikatniho jmena hrany
      */
    int arcCounter;

};

#endif // SIMULATIONSETTINGS_H
