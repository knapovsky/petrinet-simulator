/**
 * @file server.cpp
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Implementation of the tcp server
 */

#include <QtNetwork>

#include <string>

#include "server.h"

using namespace std;

/**
 * Server class constructor
 * Start listening, set ip addresses, ports
 * @param *parent pointer to parent object
 */
Server::Server(QObject *parent)
	: QTcpServer(parent){

	if(!this->listen(QHostAddress::Any, TEST_PORT)){
		if(!this->listen(QHostAddress::Any))
			qDebug() << "server not listening";
	}

	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

	for(int i = 0; i < ipAddressesList.size(); i++){
		if(ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()){
			ipAddress = ipAddressesList.at(i).toString();
			break;
		}
	}

	if(ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

	qDebug() << "The server is running on IP:"<< ipAddress
			 << "port:" << this->serverPort();



}

/**
 * Start new thread for new connection
 * @param socketDescriptor descriptor of the socket
 */
void Server::incomingConnection(int socketDescriptor){
	SimThread *simThread = new SimThread(socketDescriptor, this);
	/* ensure that thread is deleted after it has finished */
	connect(simThread, SIGNAL(finished()), simThread, SLOT(deleteLater()));
	this->moveToThread(simThread);	//TODO: check if correct!
	simThread->start();
}

SimThread::SimThread(int socketDescriptor, QObject *parent)
	: QThread(parent), socketDescriptor(socketDescriptor){
	blockSize = 0;
}

/**
 * New thread startpoint
 * Reimplementation of thread::run
 * Not optimal but according to Qt documentation.
 */
void SimThread::run(){
	tcpSocket = new QTcpSocket;

	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(processReadyRead()));

    if(!tcpSocket->setSocketDescriptor(socketDescriptor)){
        emit error(tcpSocket->error());
        return;
    }
    else
    	qDebug() << "client connected on SD: " << socketDescriptor
    	<< "local port:" << tcpSocket->localPort()
    	<< "remote port:" << tcpSocket->peerPort();


	/* Create simulation Classes */
	UserAuth userAuth;
	Transitions transitions;
	Places places;

	/* simulation needs to be initialized with startsim
	 *  command from client */
	transitions.simInit = FALSE;

	/* Create Xml parser Classes */
	writeXml = new WriteXml(&transitions, &places, tcpSocket);


	/* Process client xml */
	readXml = new ReadXml(&places, &transitions, &userAuth, writeXml, tcpSocket);


	/* Create loop - for reading messages */
	exec();

    tcpSocket->disconnectFromHost();
    qDebug() << "disconnected";
    //tcpSocket.waitForDisconnected();	//TODO: check wait...
}


/**
 * Read new socket data
 */
void SimThread::processReadyRead(){

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    /* V socketu neni zprava */
    if (tcpSocket->bytesAvailable() < (int)sizeof(quint16)){
        return;
    }

    // Pokud nebyl blockSize uz nacteny v predchozim kroku
    if(blockSize == 0){
        /* Ziskani velikosti zpravy */
        in >> blockSize;
    }

    /* Pokud zprava jeste nebyla prijata cela, ukonci se nacitani */
    if(tcpSocket->bytesAvailable() < blockSize){
        return;
    }
    else{
		QString xmldocument;
		in >> xmldocument;

		/* append data to document */
		readXml->xml.addData(xmldocument);
		blockSize = 0;
		readXml->read(xmldocument);
    }
}
