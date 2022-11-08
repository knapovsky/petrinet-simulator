/**
 * @file xmlwrite.cpp
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Classes for writing xml files.
 */

#include "xmlwrite.h"
#include <QStringList>

/**
 * Initialize pointers to other classes
 */
WriteXml::WriteXml(Transitions* PTrans, Places *PPlaces, QTcpSocket* PtcpSocket){
	pTrans = PTrans;
	pPlaces = PPlaces;
	pTcpSocket = PtcpSocket;
	xmlWr.setAutoFormatting(true);
}

/**
 * Set output parameters
 */
void WriteXml::setWriter(QIODevice *device){
	xmlWr.setDevice(device);
}

/**
 * Send list of networks to client
 * @param device - output device
 */
void WriteXml::sendNetList(QIODevice *device){

	QDomDocument documentLIST = netRepository.scanNetworks();

	QByteArray LIST;

	QDataStream outLIST(&LIST, QIODevice::WriteOnly);
	outLIST.setVersion(QDataStream::Qt_4_0);
	outLIST << (quint16)0;
	outLIST << documentLIST.toString();
	outLIST.device()->seek(0);
	outLIST << (quint16)(LIST.size() - sizeof(quint16));

	qDebug()<< "server: send list";
    //qDebug() << documentLIST.toString() << endl;
	device->write(LIST);
}

/**
 * Send Acknowledge to client
 * @param device - output device
 */
void WriteXml::sendAck(QIODevice *device){

    /* ACK XML */
    QDomDocument documentACK;
    QDomElement rootACK = documentACK.createElement("PN");
    rootACK.setAttribute("type", "ack");
    documentACK.appendChild(rootACK);
    QByteArray ACK;

    QDataStream outACK(&ACK, QIODevice::WriteOnly);
    outACK.setVersion(QDataStream::Qt_4_0);
    outACK << (quint16)0;
    outACK << documentACK.toString();
    outACK.device()->seek(0);
    outACK << (quint16)(ACK.size() - sizeof(quint16));

    device->write(ACK);

    //qDebug() << "server: send ack";
}

/**
 * Send Non-Acknowledge to client
 * @param device - output device
 */
void WriteXml::sendNAck(QIODevice *device){
    /* NACK XML */
    QDomDocument documentNACK;
    QDomElement rootNACK = documentNACK.createElement("PN");
    rootNACK.setAttribute("type", "nack");
    documentNACK.appendChild(rootNACK);
    QByteArray NACK;

    QDataStream outNACK(&NACK, QIODevice::WriteOnly);
    outNACK.setVersion(QDataStream::Qt_4_0);
    outNACK << (quint16)0;
    outNACK << documentNACK.toString();
    outNACK.device()->seek(0);
    outNACK << (quint16)(NACK.size() - sizeof(quint16));

    //qDebug()<< "server: send nack" << endl;
    device->write(NACK);

}

/**
 * Send petri - network to client
 * @param name - name of the network to send
 * @param version - version of the network
 * @param device - output device
 */
void WriteXml::sendNet(QString name, QString version, QIODevice *device){

	QString path;
	path.append("./networks/" + name + "-" + version + ".xml");

	QFile fileSend(path);
	if(fileSend.open(QIODevice::ReadOnly)){

		QDomDocument documentNET;
		documentNET.setContent(&fileSend);

		fileSend.close();

		QByteArray NET;
		QDataStream outNET(&NET, QIODevice::WriteOnly);
	    outNET.setVersion(QDataStream::Qt_4_0);
	    outNET << (quint16)0;
	    outNET << documentNET.toString();
	    outNET.device()->seek(0);
	    outNET << (quint16)(NET.size() - sizeof(quint16));

	    qDebug()<< "server: send net";
	    device->write(NET);


	}
	else{
		sendNAck(device);
	}
}

/**
 * Send update to client
 * All places that changed are sent
 * @param transitionID - ID of the active transition
 * @param device - output device
 */
void WriteXml::sendUpdate(QString transitionID, QIODevice *device){
	qDebug() << "sending update...";

	Transitions::Transition *tmpTrans;
	tmpTrans = pTrans->getTrans(transitionID);

	QMultiMap<QString, Transitions::Edge*>::iterator iter =
			tmpTrans->inputEdges.begin();

	QDomDocument documentUPDATE;

	/* Vytvoreni root elementu */
	QDomElement root = documentUPDATE.createElement("PN");
	/* Pridani root elem. do dokumentu */
	root.setAttribute("type", "netupdate");	//TODO: check name
	documentUPDATE.appendChild(root);

	/* Head */
	QDomElement head = documentUPDATE.createElement("head");
	root.appendChild(head);

	/* Body */
	QDomElement body = documentUPDATE.createElement("body");
	root.appendChild(body);

	while(iter != tmpTrans->inputEdges.end()){
		QDomElement placeID = documentUPDATE.createElement("place");
		placeID.setAttribute("id", iter.value()->pPlace->placeID);
		body.appendChild(placeID);
		for(int i = 0; i < iter.value()->pPlace->tokens.size(); i++){

			QDomElement mark = documentUPDATE.createElement("mark");
			mark.setAttribute("value", iter.value()->pPlace->tokens.at(i));
			placeID.appendChild(mark);

		}
		++iter;
	}

	iter = tmpTrans->outputEdges.begin();

	/* check all transitions and append changed places */
	while(iter != tmpTrans->outputEdges.end()){
		QDomElement placeID = documentUPDATE.createElement("place");
		placeID.setAttribute("id", iter.value()->pPlace->placeID);
		body.appendChild(placeID);
		for(int i = 0; i < iter.value()->pPlace->tokens.size(); i++){

			QDomElement mark = documentUPDATE.createElement("mark");
			mark.setAttribute("value", iter.value()->pPlace->tokens.at(i));
			placeID.appendChild(mark);

		}
		++iter;
	}

	QByteArray UPDATE;
	QDataStream outNET(&UPDATE, QIODevice::WriteOnly);
    outNET.setVersion(QDataStream::Qt_4_0);
    outNET << (quint16)0;
    outNET << documentUPDATE.toString();
    outNET.device()->seek(0);
    outNET << (quint16)(UPDATE.size() - sizeof(quint16));

    qDebug()<< "server: send net update";
    device->write(UPDATE);

}

/**
 * Send final network after simulation to client
 * @param device - output device
 */
void WriteXml::sendRunFinalUpdate(QIODevice *device){


	QDomDocument documentUPDATE;

	/* Vytvoreni root elementu */
	QDomElement root = documentUPDATE.createElement("PN");
	/* Pridani root elem. do dokumentu */
	root.setAttribute("type", "netupdate");	//TODO: check name
	documentUPDATE.appendChild(root);

	/* Head */
	QDomElement head = documentUPDATE.createElement("head");
	root.appendChild(head);

	/* Body */
	QDomElement body = documentUPDATE.createElement("body");
	root.appendChild(body);

	int j = 0;

	/* check all places and if changed send them */
	while(j < pPlaces->placeList.size()){
		QDomElement plID = documentUPDATE.createElement("place");
		plID.setAttribute("id", pPlaces->placeList[j]->placeID);
		body.appendChild(plID);
		for(int i = 0; i < pPlaces->placeList[j]->tokens.size(); i++){

			QDomElement mark = documentUPDATE.createElement("mark");
			mark.setAttribute("value", pPlaces->placeList[j]->tokens.at(i));
			plID.appendChild(mark);

		}

		++j;
	}
	QByteArray UPDATE;
	QDataStream outNET(&UPDATE, QIODevice::WriteOnly);
    outNET.setVersion(QDataStream::Qt_4_0);
    outNET << (quint16)0;
    outNET << documentUPDATE.toString();
    outNET.device()->seek(0);
    outNET << (quint16)(UPDATE.size() - sizeof(quint16));

    qDebug()<< "server: send update";
    device->write(UPDATE);

}

/**
 * If client runs simulation write it to
 * the history
 * @param net - name of the simulated network
 * @param version - version of the simulated network
 * @param user - username of the user that run simulation
 */
void WriteXml::addToHistory(QString net, QString version, QString user){
	QDir::setCurrent("./networks");

	QFile file("history.myxml");
	if(!file.open(QFile::ReadWrite | QFile::Text)){
		qDebug() << "cant open file" << endl;
		sendNAck(pTcpSocket);
		return;
	}

	/* create new QDomDocument */
	QDomDocument documentNET;
	documentNET.setContent(&file);
	QDomNodeList bodyList = documentNET.elementsByTagName("body");
	QDomNode body = bodyList.item(0);

	/* append history details */
	QDomElement child = documentNET.createElement("history");
	child.setAttribute("name", net);
	child.setAttribute("version", version);
	child.setAttribute("by", user);
	child.setAttribute("date", QDate::currentDate().toString("dd.MM.yyyy"));
	child.setAttribute("time", QTime::currentTime().toString());

	body.appendChild(child);

	QByteArray historyArray;
	historyArray.append(documentNET.toString());

	file.resize(0);
	file.seek(0);

	file.write(historyArray);
	QDir::setCurrent("..");
	documentNET.clear();

	file.close();
}
