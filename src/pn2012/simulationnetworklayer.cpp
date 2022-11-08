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
 * Implementace tridy, ktera zprostredkovava komunikaci mezi klientem a serverem v ramci simulace.
 * Pri kazdem spusteni simulace se vytvari nove spojeni a na serveru nove vlakno.
 *
 */

#include "simulationnetworklayer.h"

SimulationNetworkLayer::SimulationNetworkLayer(Settings *settingsIn, SimulationData *simDataIn)
{
    /* Potrebne ukazatele */
    this->settings = settingsIn;
    this->simData = simDataIn;

    /* Zatim nepripojeno */
    connected = false;

    // Prihlaseno v hlavni sitove vrstve
    loggedIn = true;

    blockSize = 0;
    currentState = sIDLE;
    simulationRunning = false;
    this->simulationType = tRUN;

    /* Vytvoreni TCP socketu */
    socket = new QTcpSocket(this);
    /* Navazani signalu o pripravenosti cteni na slot, ktery ziska zpravu */
    connect(socket, SIGNAL(readyRead()), this, SLOT(getMessage()));

    // Vytvoreni zprav

    /* ACK XML */
    QDomDocument documentACK;
    QDomElement rootACK = documentACK.createElement("PN");
    rootACK.setAttribute("type", "ack");
    documentACK.appendChild(rootACK);

    QDataStream outACK(&ACK, QIODevice::WriteOnly);
    outACK.setVersion(QDataStream::Qt_4_0);
    outACK << (quint16)0;
    outACK << documentACK.toString();
    outACK.device()->seek(0);
    outACK << (quint16)(ACK.size() - sizeof(quint16));

    /* NACK XML */
    QDomDocument documentNACK;
    QDomElement rootNACK = documentNACK.createElement("PN");
    rootNACK.setAttribute("type", "nack");
    documentNACK.appendChild(rootNACK);

    QDataStream outNACK(&NACK, QIODevice::WriteOnly);
    outNACK.setVersion(QDataStream::Qt_4_0);
    outNACK << (quint16)0;
    outNACK << documentNACK.toString();
    outNACK.device()->seek(0);
    outNACK << (quint16)(NACK.size() - sizeof(quint16));

    /* SIMSTART XML */
    QDomDocument documentSIMSTART;
    QDomElement rootSIMSTART = documentSIMSTART.createElement("PN");
    rootSIMSTART.setAttribute("type", "simstart");
    documentSIMSTART.appendChild(rootSIMSTART);

    QDataStream outSIMSTART(&SIMSTART, QIODevice::WriteOnly);
    outSIMSTART.setVersion(QDataStream::Qt_4_0);
    outSIMSTART << (quint16)0;
    outSIMSTART << documentSIMSTART.toString();
    outSIMSTART.device()->seek(0);
    outSIMSTART << (quint16)(SIMSTART.size() - sizeof(quint16));

    /* RUN XML */
    QDomDocument documentRUN;
    QDomElement rootRUN = documentRUN.createElement("PN");
    rootRUN.setAttribute("type", "simrun");
    documentRUN.appendChild(rootRUN);

    QDataStream outRUN(&RUN, QIODevice::WriteOnly);
    outRUN.setVersion(QDataStream::Qt_4_0);
    outRUN << (quint16)0;
    outRUN << documentRUN.toString();
    outRUN.device()->seek(0);
    outRUN << (quint16)(RUN.size() - sizeof(quint16));

    /* RESET XML */
    QDomDocument documentRESET;
    QDomElement rootRESET = documentRESET.createElement("PN");
    rootRESET.setAttribute("type", "reset");
    documentRESET.appendChild(rootRESET);

    QDataStream outRESET(&RESET, QIODevice::WriteOnly);
    outRESET.setVersion(QDataStream::Qt_4_0);
    outRESET << (quint16)0;
    outRESET << documentRESET.toString();
    outRESET.device()->seek(0);
    outRESET << (quint16)(RESET.size() - sizeof(quint16));
}

void SimulationNetworkLayer::sendACK()
{
    qDebug() << "Client -> Server: ACK, state: " << getStateString(currentState);
    socket->write(ACK);

    /*bool flushSuccess = false;
    flushSuccess = socket->flush();*/
    //qDebug() << "Flush success " << flushSuccess;
}

void SimulationNetworkLayer::sendNACK()
{
    qDebug() << "Client -> Server: NACK, state: " << getStateString(currentState);
    socket->write(NACK);

    /*bool flushSuccess = false;
    flushSuccess = socket->flush();*/
    //qDebug() << "Flush success " << flushSuccess;
}

void SimulationNetworkLayer::sendSimStart(simType typeOfSimulation)
{


    qDebug() << "Type of simulation " << typeOfSimulation;

    if(typeOfSimulation == tSTEP && !connected){

        // Typ simulace
        this->simulationType = tSTEP;

        /* Vytvoreni noveho pripojeni */
        /* Nove vlakno na serveru */
        // Vytvori se pripojeni a pokud je pripojeni uspesne, pak
        // se pokracuje v connectionEstablishe
        this->connectToServer(this->settings->ip, this->settings->port);

    }
    else if(typeOfSimulation == tRUN && !(this->simulationRunning)){

        // Odpojeni
        this->disconnectFromServer();
        connected = false;

        // Typ simulace
        this->simulationType = tRUN;

        /* Vytvoreni noveho pripojeni */
        /* Nove vlakno na serveru */
        // Vytvori se pripojeni a pokud je pripojeni uspesne, pak
        // se pokracuje v connectionEstablishe
        this->connectToServer(this->settings->ip, this->settings->port);

    }
    else if(this->simulationType == tSTEP && connected){

        // Pokud jde o krokovani a jsme uz pripojeni, tak pouze posleme step
        bar("Sending SIMSTEP");
        sendStep();
    }
    else if(simulationRunning){

        qDebug() << "Simulation is already running";
        bar("Simulation is already running");
        settings->showAlert("Simulation is already running");
        this->currentState = sIDLE;
        return;
    }
    else{

        return;
    }
}

void SimulationNetworkLayer::sendStep()
{
    /* STEP XML */
    QDomDocument documentSTEP;
    QDomElement rootSTEP = documentSTEP.createElement("PN");
    rootSTEP.setAttribute("type", "step");
    /* To, zda se jedna o prechod se kontroluje uz pri volani z akce step */

    /* TODO je potreba zjistit, zda selected item patri do teto simulace */

    if(this->settings->selectedItem != NULL && this->settings->selectedItem->type == Transition){
        rootSTEP.setAttribute("transition", this->settings->selectedItem->properties->id);
    }

    // Nebyl vybran prechod - vybere se nahodne
    else{
        bar("You havent selected transition - selecting the first found transition");
        settings->showInfo("You havent selected transition - selecting the first found transition");
        if(simData->diagramTransitions.count() > 0){
            rootSTEP.setAttribute("transition", simData->diagramTransitions.at(0)->properties->id);
        }
        else{
            bar("No transition found");
            settings->showAlert("No transition found");
        }
    }
    documentSTEP.appendChild(rootSTEP);


    // Sestaveni XML dokumentu
    QByteArray STEP;
    QDataStream outSTEP(&STEP, QIODevice::WriteOnly);
    outSTEP.setVersion(QDataStream::Qt_4_0);
    outSTEP << (quint16)0;
    outSTEP << documentSTEP.toString();
    outSTEP.device()->seek(0);
    outSTEP << (quint16)(STEP.size() - sizeof(quint16));

    this->currentState = sSTEPACK;
    qDebug() << "Client -> Server: STEP, state: " << getStateString(currentState);
    socket->write(STEP);
}

void SimulationNetworkLayer::sendRun(){

    this->currentState = sRUNACK;
    qDebug() << "Client -> Server: RUN, state: " << getStateString(currentState);
    socket->write(RUN);

}

void SimulationNetworkLayer::sendReset(){

    this->currentState = sSIMRESETACK;
    qDebug() << "Client -> Server: RESET, state: " << getStateString(currentState);
    socket->write(RESET);

}

void SimulationNetworkLayer::sendNet(QString net)
{

    // Sestaveni XML souboru
    QByteArray NET;
    QDataStream outNET(&NET, QIODevice::WriteOnly);
    outNET.setVersion(QDataStream::Qt_4_0);
    outNET << (quint16)0;
    outNET << net;
    outNET.device()->seek(0);
    outNET << (quint16)(NET.size() - sizeof(quint16));

    qDebug() << "Client -> Server: NET, state: " << getStateString(currentState);
    qDebug() << net;
    qDebug() << "\n";
    socket->write(NET);
}

void SimulationNetworkLayer::connectToServer(QString ip, QString port)
{
    if(!connected){
        /* Nejdrive odpojeni a pak nove pripojeni */
        socket->connectToHost(ip, port.toInt());
        connect(socket, SIGNAL(connected()), this, SLOT(connectionEstablished()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError()));
    }
    else{
        settings->getMainWindowPointer()->statusBar()->showMessage("Already connected. Please disconnect first");
        settings->showAlert("Already connected. Please disconnect first");
    }
}

void SimulationNetworkLayer::disconnectFromServer(){

    socket->disconnectFromHost();
    connected = false;
    loggedIn = true;
    blockSize = 0;
    simulationRunning = false;
    this->currentState = sIDLE;
}

void SimulationNetworkLayer::connectionEstablished(){

    if(this->currentState != sIDLE) return;

    this->connected = true;
    //settings->showInfo("Successfully connected to server");

    // Pokud doslo k pripojeni a simulace se ma krokovat
    if(this->simulationType == tSTEP){

        this->simulationRunning = true;
        this->currentState = sSIMSTARTACK;
        qDebug() << "Client -> Server: SIMSTART STEP, state: " << getStateString(currentState);
        bar("Sending SIMSTART for STEP");
        socket->write(SIMSTART);

    }

    // Pokud doslo k pripojeni a simulace se ma spustit
    else if(this->simulationType == tRUN){

        this->simulationRunning = true;
        this->currentState = sSIMSTARTACK;
        qDebug() << "Client -> Server: SIMSTART RUN, state: " << getStateString(currentState);
        bar("Sending SIMSTART for RUN");
        socket->write(SIMSTART);
    }
    else{
        return;
    }
}

void SimulationNetworkLayer::connectionError(){

    settings->showAlert("Connection Error");
}

MessageTypes SimulationNetworkLayer::getMessage()
{
    qDebug() << "\nNew packet, currentState " << getStateString(currentState);

    QList<QString> messages;
    QString message;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    /* V socketu neni zprava */
    if (socket->bytesAvailable() < (int)sizeof(quint16)){
        return iFAIL;
    }

    if(blockSize == 0){
        /* Ziskani velikosti zpravy */
        in >> blockSize;
    }

    //qDebug() << "Velikost prijate zpravy: " << blockSize << "V bufferu: " << socket->bytesAvailable();

    // Pokud zprava jeste nebyla prijata cela, ukonci se nacitani
    if(socket->bytesAvailable() < blockSize){
        return iFAIL;
    }
    // Prijato vice zprav najednou
    else if(socket->bytesAvailable() > blockSize){

        qDebug() << "More messages accepted";
        return iFAIL;

        char *buffer = (char*)qMalloc(sizeof(char)*blockSize);
        memset(buffer, -1, sizeof(char)*blockSize);
        uint size = blockSize;
        in.readBytes(buffer, size);

        for(int i = 0; i < blockSize; i++){
            //qDebug() << "Buffer char " << i << " : " << (int)buffer[i];
            if(i%2 == 1){
                QChar c = buffer[i];
                if((int)buffer[i] > 0){
                    message.append(c);
                }
            }
        }

        /* Pripojeni prvni zpravy */
        messages.append(message);

        qFree(buffer);

        // Dokud jsou zpravy v bufferu, tak jsou nacitany
        while(socket->bytesAvailable() > 0){

            // Nacte se velikost zpravy
            in >> blockSize;

            //qDebug() << "Blocksize dalsi zpravy:" << blockSize;

            char *buffer = (char*)malloc(sizeof(char)*blockSize);
            memset(buffer, -1, sizeof(char)*blockSize);
            uint size = blockSize;
            in.readBytes(buffer, size);

            for(int i = 0; i < blockSize; i++){
                //qDebug() << "Buffer char " << i << " : " << (int)buffer[i];
                if(i%2 == 1){
                    QChar c = buffer[i];
                    if((int)buffer[i] > 0){
                        message.append(c);
                    }
                }
            }

            //qDebug() << "Velikost prijate zpravy na konci cyklu: " << blockSize << "V bufferu: " << socket->bytesAvailable();

            message.append(message);

            qFree(buffer);
        }

        //qDebug() << "V bufferu zustalo " << socket->bytesAvailable() << "B";
    }
    /* Prijata jedna zprava */
    else if(socket->bytesAvailable() == blockSize){

        //qDebug() << "Prijata prave jedna zprava";
        // Muzeme cist z bufferu vsechna data
        in >> message;
        messages.append(message);
        blockSize = 0;

    }

    QList<QString>::iterator iter;
    for(iter = (messages.begin()); iter != (messages.end()); iter++){
        this->processMessage(*iter);
    }

    return iSUCCESS;

}

MessageTypes SimulationNetworkLayer::processMessage(QString xmldocument){

    if(!document.setContent(xmldocument)){
        /* Nevalidni XML */
        qDebug() << "Error: XML from server is not valid";
        return iFAIL;
    }

    /* Ziska prvni element */
    QDomElement root = document.firstChildElement();
    /* Ziska typ zpravy */
    QString type = root.attribute("type");
    /* Dalsi mozne atributy prvniho elementu */

    qDebug() << "Client <- Server: " << type;

    /* ACK */
    if(!type.compare("ack")){

        /* Prijat RESET ACK */
        if(this->currentState == sSIMRESETACK){

            // TODO znovunahrani simulace
            this->currentState = sIDLE;
        }

        /* Prijat STEP ACK */
        else if(this->currentState == sSTEPACK){

            this->bar("STEP command issued");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            this->currentState = sNETUPDATE;
            sendACK();

        }

        /* Prijat RUN ACK */
        else if(this->currentState == sRUNACK){

            this->bar("RUN command issued");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            this->currentState = sNETUPDATE;
            sendACK();

        }

        /* Prislo prvni ACK na prikaz SIMSTART */
        else if(this->currentState == sSIMSTARTACK){

            /* Odeslani site */
            //simData->updateNetDate();
            //simData->updateNetVersion();
            /* Posle se sit a ocekava se ACK */
            this->currentState = sSIMSTARTNETACK;
            this->bar("SIMSTART command issued");
            simData->saveCurrentNet();
            sendNet(simData->getNet().toString());
        }

        /* Prislo ACK na odeslanou sit */
        else if(this->currentState == sSIMSTARTNETACK){

            this->bar("NET uploaded");
            qDebug() << "SIMULATION TYPE = " << getSimulationTypeString(simulationType);

            /* Po SIMSTARTu se hned posilaji prikazy */
            if(simulationType == tRUN){
                sendRun();
            }
            else if(simulationType == tSTEP){
                sendStep();
            }
        }

        /* Nevyzadovany ACK */
        else{
            this->bar("ACK received, but not required");
        }
        return mACK;
    }

    /* NACK */
    else if(!type.compare("nack")){
        if(currentState == sSIMRESETACK){
            bar("Cannot reset simulation");
            settings->showAlert("Cannot reset simulation");
            currentState = sIDLE;
        }
        else if(currentState == sSTEPACK){

            bar("Transition not executable. Conditions are not met or no tokens left.");
            settings->showAlert("Transition not executable. Conditions are not met or no tokens left.");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            currentState = sIDLE;
            //sendACK();

        }
        else if(this->currentState == sRUNACK){

            this->bar("Cannot issue RUN command");
            settings->showAlert("Cannot issue RUN command");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            this->currentState = sIDLE;
            //sendACK();

        }
        else if(this->currentState == sSIMSTARTACK){


            this->currentState = sIDLE;
            this->bar("Cannot issue SIMSTART command");
            settings->showAlert("Cannot issue SIMSTART command");
        }
        else if(this->currentState == sSIMSTARTNETACK){

            this->bar("Cannot upload net in simulation start");
            settings->showAlert("Cannot upload net in simulation start");
            this->currentState = sIDLE;
        }
        else{

            bar("NACK received, but not required");
        }
        return mNACK;
    }

    /* NETUPDATE */
    else if(!type.compare("netupdate")){

        if(currentState == sNETUPDATE){

            // TODO zpracuj net update
            simData->parseUpdate(document.toString());
            bar("Net update received");
            simulationRunning = false;
            currentState = sIDLE;
            sendACK();
            socket->flush();

            // Pokud prisel update z normalni simulace,
            // pak se spojeni ukonci
            if(this->simulationType == tRUN){

                this->disconnectFromServer();
                connected = false;
            }
            else if(this->simulationType == tSTEP){

                // Simulace zustava pripojena
            }

        }
        else{

            bar("Net update received, but not required");
            currentState = sIDLE;
            sendNACK();

        }
        return mNETUPDATE;
    }

    /* Jiny typ zpravy */
    else{

        qDebug() << "Unknown message accepted";
        return iFAIL;
    }
}

void SimulationNetworkLayer::bar(QString string){

    settings->getMainWindowPointer()->statusBar()->showMessage(string, settings->statusBarTimeout);

}

bool SimulationNetworkLayer::getLoginStatus()
{
    return loggedIn;
}
