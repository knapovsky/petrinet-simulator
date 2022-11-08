/**
 * @file xmlparse.h
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Classes for parsing xml files.
 */

#ifndef XMLPARSE_H_
#define XMLPARSE_H_

#include <QXmlStreamReader>
#include <QDebug>
#include <QTcpSocket>
#include <QByteArray>


#include <iostream>
#include <string>

#include "simulation.h"
#include "netrepository.h"
#include "xmlwrite.h"

/* Stavy automatu */
enum NetworkStates {
    /* Za timto ACK uz nic neni */
    sACK = 80,
    sSIMRESET,
    sSIMRESETACK,
    sLOGIN,
    sLOGINACK,
    sGETLIST,
    sGETLISTACK,
    sLIST,
    sGETNET,
    sGETNETACK,
    sNET,
    sSIMSTEP,
    sSIMSTEPACK,
    sNETSTEP,
    sNETSTEPACK,
    sNETUPDATE,
    sSIM,
    sSIMACK,
    sNETSIMACK,
    sSAVENET,
    sSAVENETACK,
    sIDLE,
    sSIMSTART,
    sNACK
};

/**
 * Class UserAuth holds user details required
 * for login (username and password).
 */
class UserAuth{
private:
	QMap<QString, QString> users;
	QMap<QString, QString>::const_iterator iter;

public:
	static QString receivedUsername;
	static QString receivedPassword;

	/**
	 * Authenticate user - check username and password
	 * @param username name of the user
	 * @param password user password
	 */
	bool authUser(QString username, QString password);

	/**
	 * Add username and password to database
	 * @param username name of the user
	 * @param password user password
	 */
	void addUser(QString username, QString password);

	/**
	 * Add username and password from XML to DB
	 * @param username name of the user
	 * @param password user password
	 */
	void addRcvUser(QString username, QString password);
};



/**
 * Reads Xml document and parses it
 */
class ReadXml{
private:


	/* Pointers to access classes */
	Places* pPlaces;
	Transitions* pTrans;
	UserAuth* pUsers;
	WriteXml* pWriteXml;
	QTcpSocket *pTcpSocket;

	NetRepository netRepository;

	int status, lastIndex;
	QString lastName, lastVersion;

	/* Temporary variables */
	QString tmpStr, tmpStr2, xmlDoc, version, name;
	bool ok;

	/* reading of network */
	bool readPlace();
	bool readTransition();
	bool readArc();

	/* Global reading functions */
	void readAck();
	void readNack();
	void readGetList();
	void readGetNet();
	void readUser();
	void readNet();
	void readSaveNet();
	void readSimRun();
	void readSimStep();
	void readSimStart();
	void readHead();

	/**
	 * Client wants to save net on the server
	 * Network is saved into /networks directory
	 */
	void saveNet();

	/**
	 * Execute one step in simulation
	 */
	void execSimStep();

	/**
	 * Execute simulation. All transitions are scanned until
	 * no tokens are movable or simulation run time limit
	 * exceeded.
	 */
	void execSimRun();

	/**
	 * Write changes in tokens to the xml file
	 * ad sent it to the client
	 */
	void execSimStepUpdate();


public:
	/* Main streaming variable */
	QXmlStreamReader xml;

	/**
	 * Create pointers to places and transitions
	 * @param *PPlac pointer to Places
	 * @param *PTrans pointer to Transitions
	 * @param *PUsers pointer to users class
	 * @param *PWriteXml pointer to write xml clase
	 * @param *PtcpSocket pointer to socket
	 */
	ReadXml(Places* PPlac, Transitions* PTrans, UserAuth *PUsers,
			WriteXml* PWriteXml, QTcpSocket* PtcpSocket);

	/**
	 * Set reading source and check Xml file
	 * @param *device pointer to reading device
	 * @return 1 on success 0 on failure
	 */
	bool read(QString xmldocument);
};



#endif /* XMLPARSE_H_ */
