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
 * Tato trida zprostredkovava komunikaci mezi klientem a serverem v ramci simulace.
 * Pri kazdem spusteni simulace se vytvari nove spojeni a na serveru nove vlakno.
 *
 */

#ifndef SIMULATIONNETWORKLAYER_H
#define SIMULATIONNETWORKLAYER_H

//#include <QtCore>
//#include <QtGui>
//#include <QtXml>
//#include <QtNetwork/QtNetwork>
//#include <QTcpSocket>
#include <simulationdata.h>
#include <settings.h>
#include <simlist.h>
#include <networkbrowser.h>
#include <networklayer.h>
#include "networkenum.h"
#include <simulationdata.h>


class SimulationNetworkLayer : public QObject
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *settingsIn Ukazatel na globalni nastaveni programu
      * @param *simDataIn Ukazatel na data simulace
      * @return Ukazatel na nove vytvoreny objekt
      */
    SimulationNetworkLayer(Settings *settingsIn, SimulationData *simDataIn);

    /**
      * Odesle ACK zpravu
      */
    void sendACK();

    /**
      * Odesle NACK zpravu
      */
    void sendNACK();

    /**
      * Odesle SIMSTART zpravu pro spusteni simulace
      * @param typeOfSimulation Typ simulace
      */
    void sendSimStart(simType typeOfSimulation);

    /**
      * Odesle STEP zpravu
      */
    void sendStep();

    /**
      * Odesle RUN zpravu
      */
    void sendRun();

    /**
      * Odesle serveru RESET zpravu pro reset simulace
      */
    void sendReset();

    /**
      * Odesle sit na server
      * @param net Odesilana sit
      */
    void sendNet(QString net);

    /**
      * Pripoji k serveru
      * @param ip IP adresa serveru
      * @param port Port na serveru
      */
    void connectToServer(QString ip, QString port);

    /**
      * Odpoji od serveru
      */
    void disconnectFromServer();

    /**
      * Vypise hlasku do status baru
      * @param string Hlaska, ktera ma byt vypsana
      */
    void bar(QString string);

    /**
      * Je uzivatel prihlaseny?
      * @return Indikace toho, zda je uzivatel prihlaseny k serveru
      */
    bool getLoginStatus();

    /**
      * Zpracuje prijatou zpravu
      * @param xmldocument Prijata zprava
      */
    MessageTypes processMessage(QString xmldocument);

public slots:

    /**
      * Ziska prijatou zpravu z bufferu - pri signalu o prijeti dat
      * @return Ziskany typ zpravy
      */
    MessageTypes getMessage();

    /**
      * Slot, ktery je volan pri uspesnem pripojeni na server
      */
    void connectionEstablished();

    /**
      * Slot, ktery je volan pri chybe spojeni
      */
    void connectionError();

private:

    /* XML dokumenty */

    QByteArray ACK;
    QByteArray NACK;
    QByteArray RUN;
    QByteArray RESET;
    QByteArray SIMSTART;

    /**
      * Uzivatelske jmeno
      */
    QString username;

    /**
      * Uzivatelske heslo
      */
    QString password;

    /**
      * Jmeno site
      */
    QString name;

    /**
      * Verze site
      */
    QString version;

    /**
      * Autor site
      */
    QString author;

    /**
      * Datum vytvoreni site
      */
    QString date;

    /**
      * Zprava
      */
    QDomDocument document;

    /**
      * Indikace toho, zda je uzivatel pripojen
      */
    bool connected;

    /**
      * Indikace toho, zda je uzivatel prihlasen
      */
    bool loggedIn;

    /**
      * Deskriptor socketu
      */
    QTcpSocket *socket;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na data simulace
      */
    SimulationData *simData;

    /**
      * Velikost prijate zpravy
      */
    quint16 blockSize;

    /**
      * Aktualni stav stavoveho automatu
      */
    NetworkStates currentState;

    /**
      * Indikace toho, zda je dana simulace na serveru spustena
      */
    bool simulationRunning;

    /**
      * Typ simulace tRUN||tSTEP
      */
    simType simulationType;
};

#endif // SIMULATIONNETWORKLAYER_H
