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
 * Trida reprezentujici simulaci a vse potrebne pro jeji zobrazeni.
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

//#include <QtGui>
//#include <QtCore>
//#include <QWidget>
//#include <QList>
#include "diagramscene.h"
#include "diagramproperties.h"
#include "simulationdata.h"
#include "diagramitem.h"
//#include <QDebug>

class DiagramItem;
class SimulationNetworkLayer;

class Simulation : public QWidget
{

   Q_OBJECT

public:

    /**
      * Konstruktor
      * Pouziva se bud fileName_in, nebo simFile. Ne oba parametry zaroven.
      * @param fileName_in Jmeno souboru, ktery obsahuje obsah simulace
      * @param *modeObjectIn Ukazatel na globalni nastaveni programu
      * @param simFile Obsah simulacniho souboru
      * @return Ukazatel na nove vytvoreny objekt simulace
      */
    Simulation(QString fileName_in, Settings *modeObjectIn, QString simFile);

    /**
      * Vrati jmeno simulace
      * @return Retezec se jmenem simulace
      */
    QString getFilename();

    /**
      * Obsluzna funkce pri vyberu objektu simulacni sceny
      */
    void itemSelected(DiagramItem* item);

    /**
      * Odpoji simulaci od serveru
      */
    void disconnectFromServer();

public slots:

    /**
      * Spusti simulaci na serveru
      */
    void runSimulation();

    /**
      * Spusti krokovani na serveru
      */
    void nextStepSimulation();

    /**
      * Ulozi simulaci lokalne
      */
    int saveSimulationLocally();

    /**
      * Ulozi simulaci lokalne na zadanem umisteni
      */
    int saveAsSimulationLocally();

    /**
      * Ulozi simulaci na server
      */
    int saveSimulationToServer();

    /**
      * Restartuje simulaci na serveru
      */
    int restartSimulation();

    /**
      * Odstrani simulaci
      */
    void removeSimulation();

    /**
      * Aktualizuje simulacni scenu
      */
    void updateScene();

    /**
      * Vrati ukazatel na simulacni scenu
      * @return Ukazatel na scenu simulace
      */
    DiagramScene* getScene();

    /**
      * Vrati ukazatel na objekt obsahujici simulacni data
      * @return Ukazatel na objekt se simulacnimi daty
      */
    SimulationData* getSimData();

    /**
      * Funkce upravujici pomer velikosti mezi scenou a vlastnostmi vybraneho prvku
      */
    void resizeProps();

signals:

    /**
      * Chyba simulace
      */
    void simulationError();

private:

    /**
      * Vytvori vnitrni widgety simulace - scena, vlastnosti vybraneho prvku
      */
    void createSimulationWidgets();

    /**
      * Hlavni layout tohoto widgetu
      */
    QVBoxLayout *simulationLayout;

    /**
      * Simulacni scena
      */
    QGraphicsView *graphView;

    /**
      * Rozdelovac mezi scenou a vlastnostmi vybraneho prvku
      */
    QSplitter *splitter;

    /**
      * Simulacni scena
      */
    DiagramScene *scene;

    /**
      * Ukazatel na objekt zobrazujici vlastnosti vybraneho prvku simulacni sceny
      */
    DiagramProperties *properties;

    /**
      * Sitova vrstva simulace
      */
    SimulationNetworkLayer *simNetLayer;

    /**
      * Globalni nastaveni programu
      */
    Settings *modeObject;

    /**
      * Simulacni data
      */
    SimulationData *simData;
};

#endif // SIMULATION_H
