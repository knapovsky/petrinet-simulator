/**
 * @file xmlwrite.h
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

#ifndef XMLWRITE_H_
#define XMLWRITE_H_

#include <QXmlStreamWriter>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QMultiMap>
#include <QtXml>
#include <QDir>
#include <QTcpSocket>
#include <QThread>


#include "simulation.h"
#include "netrepository.h"


#define WAITTIME 3000

/**
 * Write Xml document to output device
 */
class WriteXml{
public:

	/**
	 * Initialize pointers to other classes
	 */
	WriteXml(Transitions* PTrans, Places *PPlace, QTcpSocket* PtcpSocket);

	/**
	 * Set output parameters
	 */
	void setWriter(QIODevice *device);

	/**
	 * Send list of networks to client
	 * @param device - output device
	 */
	void sendNetList(QIODevice *device);

	/**
	 * Send Acknowledge to client
	 * @param device - output device
	 */
	void sendAck(QIODevice *device);

	/**
	 * Send Non-Acknowledge to client
	 * @param device - output device
	 */
	void sendNAck(QIODevice *device);

	/**
	 * Send petri - network to client
	 * @param name - name of the network to send
	 * @param version - version of the network
	 * @param device - output device
	 */
	void sendNet(QString name, QString version, QIODevice *device);

	/**
	 * Send update to client
	 * All places that changed are sent
	 * @param transitionID - ID of the active transition
	 * @param device - output device
	 */
	void sendUpdate(QString transitionID, QIODevice *device);

	/**
	 * Send final network after simulation to client
	 * @param device - output device
	 */
	void sendRunFinalUpdate(QIODevice *device);

	/**
	 * If client runs simulation write it to
	 * the history
	 * @param net - name of the simulated network
	 * @param version - version of the simulated network
	 * @param user - username of the user that run simulation
	 */
	void addToHistory(QString net, QString version, QString user);

private:
	QXmlStreamWriter xmlWr;
	Transitions* pTrans;
	Places *pPlaces;
	QTcpSocket *pTcpSocket;
	NetRepository netRepository;
};

#endif /* XMLWRITE_H_ */
