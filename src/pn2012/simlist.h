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
 * Definice tridy, ktera uchovava seznam aktualne otevrenych simulaci, umoznuje
 * s nimi pracovat, vytvaret nove a odstranovat je.
 *
 */

#ifndef SIMLIST_H
#define SIMLIST_H

//#include <QtCore>
//#include <QtGui>
//#include <QWidget>
//#include <QList>
#include "simulation.h"

class SimList : public QTabWidget{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *modeObjectIn Ukazatel na globalni nastaveni programu
      */
    SimList(Settings *modeObjectIn);

    /**
      * Prida novou simulaci
      * @param fileName Jmeno simulace
      * @param *modeObjectIn Ukazatel na globalni nastaveni programu
      * @param simFile Obsah simulacniho souboru
      * @return Ukazatel na nove vytvorenou simulaci
      */
    Simulation *addSimulation(QString fileName, Settings *modeObjectIn, QString simFile);

    /**
      * Aktualne vybrana simulace
      */
    Simulation* currentSimulation;

    /**
      * Odpoji vsechny simulace ze serveru
      */
    void disconnectFromServer();

public slots:

    /**
      * Volano pri vyberu polozky ve scene, nastavuje potrebne ukazatele v glob. nastaveni.
      */
    void itemSelected(DiagramItem *);

    /**
      * Zmena simulace
      * @param i Index simulace
      */
    void changeCurrentSimulation(int i);

    /**
      * Zmeni aktualni simulaci
      * @param *sim Ukazatel na simulaci, ktera bude vzata jako aktualni
      */
    void changeCurrentSimulation(Simulation *sim);

    /**
      * Obsluhuje nezdar pri vytvoreni nove simulace
      */
    void simCreationError();

    /**
      * Zavre simulaci
      * @param i Index simulace, ktera se ma zavrit
      */
    void closeTab(int i);

signals:

    /**
      * Signalizace toho, ze byla vybrana polozka simulace
      * - Misto, Prechod, nebo Hrana
      * @param Ukazatel na vybranou polozku
      */
    void itemSelectedSignal(DiagramItem *);

private:

    /**
      * Seznam otevrenych simulaci
      */
    QList<Simulation*> simulations;

    /**
      * Aktualni simulace
      */
    Settings *settings;

    /**
      * Pomocny ukazatel na nove vytvorenou simulaci
      */
    Simulation *newSim;
};

#endif // SIMLIST_H
