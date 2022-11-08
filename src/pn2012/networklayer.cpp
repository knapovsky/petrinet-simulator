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
 * Implementace tridy umoznujici sitovou komunikaci. Tato trida je zahrnuje ridici
 * komunikaci. Komunikace v ramci simulace je obsazena ve tride SimulationNetworkLayer.
 *
 */

#include "networklayer.h"

NetworkLayer::NetworkLayer(Settings *settingsIn, SimList *simListIn)
{
    /* Ukazatel na nastaveni programu */
    settings = settingsIn;
    simList = simListIn;

    /* Zatim nepripojeno */
    connected = false;
    loggedIn = false;

    // Velikost zpravy
    blockSize = 0;

    // Stav automatu
    currentState = sIDLE;

    // Simulace nebezi
    simulationRunning = false;
    this->simulationType = tRUN;

    /* Ziskani ip adresy mistniho sitoveho zarizeni */
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    /* Vezme se prvni nalezena non-localhost adresa */
    for (int i = 0; i < ipAddressesList.size(); ++i) {

        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    /* Pokud nebyla adresa nalezena, bere se adresa lokalni */
    if (ipAddress.isEmpty()){

        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    /* Predvyplneni IP adresy */
    settings->ip = ipAddress;
    //settings->ip = "192.168.1.110";
    //settings->port = "44444";
    //settings->username = "testuser";
    //settings->password = "password";

    /* Vytvoreni TCP socketu */
    socket = new QTcpSocket(this);

    /* Navazani signalu o pripravenosti cteni na slot, ktery ziska zpravu */
    connect(socket, SIGNAL(readyRead()), this, SLOT(getMessage()));

    //
    // Vytvoreni XML dokumentu
    //

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

    /* GETLIST XML */
    QDomDocument documentGETLIST;
    QDomElement rootGETLIST = documentGETLIST.createElement("PN");
    rootGETLIST.setAttribute("type", "getlist");
    documentGETLIST.appendChild(rootGETLIST);

    QDataStream outGETLIST(&GETLIST, QIODevice::WriteOnly);
    outGETLIST.setVersion(QDataStream::Qt_4_0);
    outGETLIST << (quint16)0;
    outGETLIST << documentGETLIST.toString();
    outGETLIST.device()->seek(0);
    outGETLIST << (quint16)(GETLIST.size() - sizeof(quint16));

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

    /* SAVENET XML */
    QDomDocument documentSAVENET;
    QDomElement rootSAVENET = documentSAVENET.createElement("PN");
    rootSAVENET.setAttribute("type", "savenet");
    documentSAVENET.appendChild(rootSAVENET);

    QDataStream outSAVENET(&SAVENET, QIODevice::WriteOnly);
    outSAVENET.setVersion(QDataStream::Qt_4_0);
    outSAVENET << (quint16)0;
    outSAVENET << documentSAVENET.toString();
    outSAVENET.device()->seek(0);
    outSAVENET << (quint16)(SAVENET.size() - sizeof(quint16));

    /* RUN XML */
    QDomDocument documentRUN;
    QDomElement rootRUN = documentRUN.createElement("PN");
    rootRUN.setAttribute("type", "run");
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

void NetworkLayer::sendACK()
{
    qDebug() << "Client -> Server: ACK, state: " << getStateString(currentState);
    socket->write(ACK);

    /*bool flushSuccess = false;
    flushSuccess = socket->flush();*/
    //qDebug() << "Flush success " << flushSuccess;
}

void NetworkLayer::sendNACK()
{
    qDebug() << "Client -> Server: NACK, state: " << getStateString(currentState);
    socket->write(NACK);

    /*bool flushSuccess = false;
    flushSuccess = socket->flush();*/
    //qDebug() << "Flush success " << flushSuccess;
}

void NetworkLayer::sendLogin()
{
    /* LOGIN XML */
    QDomDocument documentLOGIN;
    QDomElement rootLOGIN = documentLOGIN.createElement("PN");
    rootLOGIN.setAttribute("type", "login");
    rootLOGIN.setAttribute("username", settings->username);
    rootLOGIN.setAttribute("password", settings->password);
    documentLOGIN.appendChild(rootLOGIN);

    QByteArray LOGIN;
    QDataStream outLOGIN(&LOGIN, QIODevice::WriteOnly);
    outLOGIN.setVersion(QDataStream::Qt_4_0);
    outLOGIN << (quint16)0;
    outLOGIN << documentLOGIN.toString();
    outLOGIN.device()->seek(0);
    outLOGIN << (quint16)(LOGIN.size() - sizeof(quint16));

    /* Ocekava se ACK/NACK */
    this->currentState = sLOGINACK;
    qDebug() << "Client -> Server: LOGIN, state: " << getStateString(currentState);
    socket->write(LOGIN);
}

void NetworkLayer::sendGetList()
{
    this->currentState = sGETLISTACK;
    qDebug() << "Client -> Server: GETLIST, state: " << getStateString(currentState);
    socket->write(GETLIST);  
}

void NetworkLayer::sendGetNet(QString netName, QString version)
{
    /* GETNET XML */
    QDomDocument documentGETNET;
    QDomElement rootGETNET = documentGETNET.createElement("PN");
    rootGETNET.setAttribute("type", "getnet");
    rootGETNET.setAttribute("name", netName);
    rootGETNET.setAttribute("version", version);
    documentGETNET.appendChild(rootGETNET);

    QByteArray GETNET;
    QDataStream outGETNET(&GETNET, QIODevice::WriteOnly);
    outGETNET.setVersion(QDataStream::Qt_4_0);
    outGETNET << (quint16)0;
    outGETNET << documentGETNET.toString();
    outGETNET.device()->seek(0);
    outGETNET << (quint16)(GETNET.size() - sizeof(quint16));

    this->currentState = sGETNETACK;
    qDebug() << "Client -> Server: GETNET, state: " << getStateString(currentState);
    socket->write(GETNET);
}

void NetworkLayer::sendSimStart(simType typeOfSimulation)
{
    //qDebug() << "Type of simulation " << typeOfSimulation;

    // Simulace nebezi, chceme step simulaci
    if(typeOfSimulation == tSTEP && !(this->simulationRunning)){

        this->simulationType = tSTEP;
        this->simulationRunning = true;
        this->currentState = sSIMSTARTACK;
        qDebug() << "Client -> Server: SIMSTART STEP, state: " << getStateString(currentState);
        socket->write(SIMSTART);

    }

    // Simulace nebezi, chceme run simulaci
    else if(typeOfSimulation == tRUN && !(this->simulationRunning)){

        this->simulationType = tRUN;
        this->simulationRunning = true;
        this->currentState = sSIMSTARTACK;
        qDebug() << "Client -> Server: SIMSTART RUN, state: " << getStateString(currentState);
        socket->write(SIMSTART);

    }

    // Simulace bezi, chceme step
    else if(this->simulationType == tSTEP && this->simulationRunning){

        sendStep();
    }

    // Simulace bezi, chceme run - error
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

void NetworkLayer::sendStep()
{
    /* STEP XML */
    QDomDocument documentSTEP;
    QDomElement rootSTEP = documentSTEP.createElement("PN");
    rootSTEP.setAttribute("type", "step");

    /* To, zda se jedna o prechod se kontroluje uz pri volani z akce step */
    if(this->settings->selectedItem != NULL && this->settings->selectedItem->type == Transition){

        rootSTEP.setAttribute("transition", this->settings->selectedItem->properties->id);
    }
    else{
        bar("You havent selected transition - selecting the first found transition");
        settings->showAlert("You havent selected transition - selecting the first found transition");
        if(this->settings->simList->currentSimulation->getSimData()->diagramTransitions.count() > 0){

            rootSTEP.setAttribute("transition",
                                  this->settings->simList->currentSimulation->getSimData()->diagramTransitions.at(0)->properties->id);
        }
        else{
            bar("No transition found");
            settings->showAlert("No transition found");
        }
    }
    documentSTEP.appendChild(rootSTEP);

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

void NetworkLayer::sendRun(){

    this->currentState = sRUNACK;
    qDebug() << "Client -> Server: RUN, state: " << getStateString(currentState);
    socket->write(RUN);
}

void NetworkLayer::sendSaveNet()
{
    this->currentState = sSAVENETACK;
    qDebug() << "Client -> Server: SAVENET, state: " << getStateString(currentState);
    socket->write(SAVENET);
}

void NetworkLayer::sendReset()
{
    this->currentState = sSIMRESETACK;
    qDebug() << "Client -> Server: RESET, state: " << getStateString(currentState);
    socket->write(RESET);
}

void NetworkLayer::sendNet(QString net)
{
    //qDebug() << "Sending NET: \n";
    //qDebug() << net;
    //qDebug() << "\n";

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

void NetworkLayer::connectToServer(QString ip, QString port)
{
    if(!connected){

        /* Nejdrive odpojeni a pak nove pripojeni */
        socket->connectToHost(ip, port.toInt());
        connect(socket, SIGNAL(connected()), this, SLOT(connectionEstablished()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError()));
    }
    else if(connected && !loggedIn){

        sendLogin();
    }
    else{

        settings->getMainWindowPointer()->statusBar()->showMessage("Already connected. Please disconnect first");
        settings->showAlert("Already connected. Please disconnect first");
    }
}

void NetworkLayer::disconnectFromServer(){

    socket->disconnectFromHost();
    connected = false;
    loggedIn = false;
    blockSize = 0;
    this->currentState = sIDLE;

    bar("You have been disconnected from server.");
    //settings->showInfo("You have been disconnected from server.");
}

MessageTypes NetworkLayer::getMessage()
{
    qDebug() << "New packet, currentState = " << getStateString(currentState);

    QList<QString> messages;
    QString message;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    /* V socketu neni zprava */
    if (socket->bytesAvailable() < (int)sizeof(quint16)){
        return iFAIL;
    }

    // Pokud nebyl blockSize uz nacteny v predchozim kroku
    if(blockSize == 0){
        /* Ziskani velikosti zpravy */
        in >> blockSize;
    }

    // Pokud zprava jeste nebyla prijata cela, ukonci se nacitani
    // Zprava bude nactena v krocich pristich
    if(socket->bytesAvailable() < blockSize){
        return iFAIL;
    }

    // Prijato vice zprav najednou - nyni by jiz nemelo nastavat
    else if(socket->bytesAvailable() > blockSize){

        qDebug() << "More messages accepted!";
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

            char *buffer = (char*)malloc(sizeof(char)*blockSize);
            memset(buffer, -1, sizeof(char)*blockSize);
            uint size = blockSize;
            in.readBytes(buffer, size);

            for(int i = 0; i < blockSize; i++){

                if(i%2 == 1){
                    QChar c = buffer[i];
                    if((int)buffer[i] > 0){
                        message.append(c);
                    }
                }
            }


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

void NetworkLayer::connectionEstablished(){

    //settings->showInfo("Succesfully connected to server");
    connected = true;
    this->sendLogin();
}

void NetworkLayer::connectionError(){

    settings->showAlert("Connection Error");
}

MessageTypes NetworkLayer::processMessage(QString xmldocument){



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

    qDebug() << "Client <- Server: " << type ;
    //
    //  STAVOVY AUTOMAT
    //  Porovnavat s diagramem
    //

    // PRIJATO ACK
    if(!type.compare("ack")){

        if(this->currentState == sSIMRESETACK){

            // TODO znovunahrani simulace
            this->currentState = sIDLE;
        }
        else if(this->currentState == sLOGINACK){

            this->bar("Logged in");
            settings->showInfo("Logged in");
            loggedIn = true;
            this->currentState = sIDLE;
        }
        else if(this->currentState == sGETLISTACK){

            this->bar("GETLIST command issued");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            this->currentState = sLIST;
            sendACK();
        }
        else if(this->currentState == sGETNETACK){

            bar("GETNET command issued");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            this->currentState = sNET;
            sendACK();
        }
        else if(this->currentState == sSTEPACK){

            this->bar("STEP command issued");
            this->currentState = sNETUPDATE;
        }
        else if(this->currentState == sNETSTEPACK){

            this->bar("NET uploaded");
            this->currentState = sNETUPDATE;
        }
        else if(this->currentState == sSIMACK){

            this->bar("SIM command issued");
            /* TODO poslat sit */
            settings->simList->currentSimulation->getSimData()->updateNetDate();
            settings->simList->currentSimulation->getSimData()->updateNetVersion();
            this->currentState = sNETSIMACK;
            sendNet(settings->simList->currentSimulation->getSimData()->getNet().toString());
        }
        else if(this->currentState == sNETSIMACK){

            this->bar("NET uploaded");
            this->currentState = sNETUPDATE;
        }
        else if(this->currentState == sSAVENETACK){

            this->bar("SAVENET command issued");
            /* TODO poslat sit */
            settings->simList->currentSimulation->getSimData()->updateNetDate();
            settings->simList->currentSimulation->getSimData()->updateNetVersion();
            this->currentState = sNETSAVEACK;
            sendNet(settings->simList->currentSimulation->getSimData()->getNet().toString());

        }
        else if(this->currentState == sNETSAVEACK){

            this->bar("NET uploaded");
            settings->showInfo("Network saved on server");
            this->currentState = sIDLE;
        }
        else if(this->currentState == sRUNACK){

            this->bar("RUN command issued");
            this->currentState = sNETUPDATE;
        }
        else if(this->currentState == sSIMSTARTACK){

            /* Odeslani site */
            settings->simList->currentSimulation->getSimData()->updateNetDate();
            settings->simList->currentSimulation->getSimData()->updateNetVersion();
            this->currentState = sSIMSTARTNETACK;
            sendNet(settings->simList->currentSimulation->getSimData()->getNet().toString());
            this->bar("SIMSTART command issued");
        }
        else if(this->currentState == sSIMSTARTNETACK){

            this->bar("NET uploaded");
            //qDebug() << "SIMULATION TYPE = " << this->simulationType;
            if(this->simulationType == tRUN){
                sendRun();
            }
            else if(this->simulationType == tSTEP){

                sendStep();
            }
        }
        else if(this->currentState == sLISTACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else if(this->currentState == sNETACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else if(this->currentState == sNETUPDATEACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else{

            this->bar("ACK received, but not required");
        }
        return mACK;
    }

    // PRIJATO NACK
    else if(!type.compare("nack")){

        if(currentState == sSIMRESETACK){

            bar("Cannot reset simulation");
            settings->showAlert("Cannot reset simulation");
            currentState = sIDLE;
        }
        else if(currentState == sLOGINACK){

            loggedIn = false;
            currentState = sIDLE;
            this->bar("Cannot log in. Check your account information.");
            settings->showAlert("Cannot log in. Check your account information.");
            this->disconnectFromServer();
        }
        else if(currentState == sGETLISTACK){

            bar("Cannot issue GETLIST command");
            settings->showAlert("Cannot issue GETLIST command");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            currentState = sIDLE;
            //sendACK();

        }
        else if(currentState == sGETNETACK){

            bar("Cannot issue GETNET command");
            settings->showAlert("Cannot issue GETNET command");
            /* Tady se posle ACK, ktere server prijme */
            /* Kvuli tomu, aby nechodily od serveru 2 zpravy za sebou */
            currentState = sIDLE;
            //sendACK();

        }
        else if(currentState == sSTEPACK){

            bar("Cannot issue STEP command");
            settings->showAlert("Cannot issue STEP command");
            currentState = sIDLE;
        }
        else if(currentState == sNETSTEPACK){

            bar("Cannot upload net for step simulation");
            settings->showAlert("Cannot upload net for step simulation");
            currentState = sIDLE;
        }
        else if(currentState == sSIMACK){

            bar("Cannot issue SIM command");
            settings->showAlert("Cannot issue SIM command");
            currentState = sIDLE;
        }
        else if(currentState == sNETSIMACK){

            bar("Cannot upload net to server");
            settings->showAlert("Cannot upload net to server");
            currentState = sIDLE;
        }
        else if(currentState == sSAVENETACK){

            bar("Cannot issue SAVENET command");
            settings->showAlert("Cannot issue SAVENET command");
            currentState = sIDLE;
        }
        else if(currentState == sNETSAVEACK){

            bar("Cannot upload net to server");
            settings->showAlert("Cannot upload net to server");
            currentState = sIDLE;
        }
        else if(this->currentState == sRUNACK){

            this->bar("Cannot issue RUN command");
            settings->showAlert("Cannot issue RUN command");
            this->currentState = sIDLE;
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
        else if(this->currentState == sLISTACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else if(this->currentState == sNETACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else if(this->currentState == sNETUPDATEACKACK){

            // Zamezeni dvou zprav po sobe
            this->currentState = sIDLE;
        }
        else{
            bar("NACK received, but not required");
        }
        return mNACK;
    }

    // PRIJATA ZPRAVA GETLIST
    else if(!type.compare("getlist")){

        return mGETLIST;
    }

    // PRIJATA ZPRAVA GETNET
    else if(!type.compare("getnet")){

        name = root.attribute("name");
        version = root.attribute("version");
        return mGETNET;
    }

    // PRIJATA ZPRAVA LIST
    else if(!type.compare("list")){

        //qDebug() << "Current state in list " << currentState;
        if(currentState == sLIST){

            /* TODO zpracovat list */
            // LIST JE ULOZEN V document promenne

            // TODO tady ziskam dokument ze sitove vstrvy
            // Vytvoreni dialogu
            NetworkBrowser *browserDialog = new NetworkBrowser(settings);
            // Naplneni browserDialogu
            browserDialog->setBrowser(document);
            browserDialog->show();

            bar("List received");
            currentState = sLISTACKACK;
            sendACK();  
        }
        else{

            bar("List received, but not required");
            // TODO ? posilat NACK nebo ACK pri spatne vyslane zprave?
            currentState = sIDLE;
            sendNACK();

        }
        return mLIST;
    }

    // PRIJATA ZPRAVA LOGIN
    else if(!type.compare("login")){

        username = root.attribute("username");
        password = root.attribute("password");
        return mLOGIN;
    }

    // PRIJATA ZPRAVA NET
    else if(!type.compare("net")){

        if(currentState == sNET){
            bar("Net received");
            settings->simList->addSimulation("", settings, document.toString());
            //simList->addSimulation("", settings, document.toString());
            currentState = sNETACKACK;
            sendACK();

        }
        else{

            bar("Net received but not required");
            currentState = sIDLE;
            sendNACK();

        }
        return mNET;
    }

    // PRIJATA ZPRAVA NETUPDATE
    else if(!type.compare("netupdate")){

        if(currentState == sNETUPDATE){
            // TODO zpracuj net update
            settings->simList->currentSimulation->getSimData()->parseUpdate(document.toString());
            bar("Net update received");
            currentState = sNETUPDATEACKACK;
            sendACK();

        }
        else{

            bar("Net update received, but not required");
            currentState = sIDLE;
            sendNACK();

        }
        return mNETUPDATE;
    }

    // PRIJATA ZPRAVA SAVENET
    else if(!type.compare("savenet")){

        return mSAVENET;
    }

    // PRIJATA ZPRAVA SIM
    else if(!type.compare("sim")){

        return mSIM;
    }

    // PRIJATA ZPRAVA SIMSTEP
    else if(!type.compare("simstep")){

        return mSIMSTEP;
    }

    else{
        qDebug() << "Unknown message accepted";
        return iFAIL;
    }
}

void NetworkLayer::bar(QString string){

    settings->getMainWindowPointer()->statusBar()->showMessage(string, settings->statusBarTimeout);
}

bool NetworkLayer::getLoginStatus()
{
    return loggedIn;
    //return true;
}
