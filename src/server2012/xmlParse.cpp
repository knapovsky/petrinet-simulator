/**
 * @file xmlparse.cpp
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

#include "xmlParse.h"

using namespace std;

QString UserAuth::receivedUsername = "";
QString UserAuth::receivedPassword = "";

/**
 * Create pointers to places and transitions
 * @param *PPlac pointer to Places
 * @param *PTrans pointer to Transitions
 * @param *PUsers pointer to users class
 * @param *PWriteXml pointer to write xml clase
 * @param *PtcpSocket pointer to socket
 */
ReadXml::ReadXml(Places* PPlac, Transitions* PTrans,
		UserAuth* PUsers, WriteXml* PWriteXml, QTcpSocket* PtcpSocket){
	pPlaces = PPlac;
	pTrans = PTrans;
	pUsers = PUsers;
	pWriteXml = PWriteXml;
	pTcpSocket = PtcpSocket;

	pUsers->addUser("testuser", "password");
}

/**
 * Set reading source and check Xml file
 * @param *device pointer to reading device
 * @return 1 on success 0 on failure
 */
bool ReadXml::read(QString xmldocument){
	QString xmlType;

	/* do backup */
	xmlDoc = xmldocument;

	if(xml.readNextStartElement()){

		QXmlStreamAttributes attributes = xml.attributes();
			if(attributes.hasAttribute("type")){
				xmlType = attributes.value("type").toString();

				if(xmlType == "login")
					readUser();
				else if(xmlType == "ack")
					readAck();
				else if(xmlType == "nack")
					readNack();
				else if(xmlType == "getnet")
					readGetNet();
				else if(xmlType == "getlist")
					readGetList();
				else if(xmlType == "net")
					readNet();
				else if(xmlType == "savenet")
					readSaveNet();
				else if(xmlType == "simrun")
					readSimRun();
				else if(xmlType == "step")
					readSimStep();
				else if(xmlType == "simstart")
					readSimStart();
				else{
					xml.clear();
					return 0;
				}
			}

		else{
			xml.clear();
			return 0;
		}
	}
	xml.clear();
	return 1;
}

/**
 * Reads Xml file - call appropriate functions
 * for every tag - place, transition, arc
 */
void ReadXml::readNet(){
	/* Read until next start element */
	qDebug() << "reading network";

	while(xml.readNextStartElement()){

		if(xml.name() == "place")
			readPlace();
		else if(xml.name() == "transition")
			readTransition();
		else if(xml.name() == "arc"){
			if(!readArc()) return;
		}
		else if(xml.name() == "body")
			xml.readNext();
		else if(xml.name() == "head")
			readHead();
		else{
			xml.skipCurrentElement();
		}
	}
	if(status == sSAVENET){
		qDebug() << "saving network";
		saveNet();
	}
	else if(pTrans->simInit == true){
		pWriteXml->sendAck(pTcpSocket);

		if(status == sSIMSTART){
			pWriteXml->addToHistory(lastName, lastVersion, pUsers->receivedUsername);
		}
	}
}

/**
 * Read head of the xml file
 */
void ReadXml::readHead(){
	while(xml.readNextStartElement()){
		if(xml.name() == "version"){
			version = xml.readElementText();
			lastVersion = version;
			//xml.skipCurrentElement();
		}
		else if(xml.name() == "name"){
			name = xml.readElementText();
			lastName = name;
			//xml.skipCurrentElement();
		}
		else
			xml.skipCurrentElement();
	}
}

/**
 * Read place and included tokens
 * @return 1 on success 0 on failure
 */
bool ReadXml::readPlace(){
	/* Read place ID */
	QXmlStreamAttributes attributes = xml.attributes();
	if(attributes.hasAttribute("id")) {
		QString id = attributes.value("id").toString();

		/* Add place with appropriate ID to placeList */
		pPlaces->addPlace(id);
	}
	else
		return FALSE;

	/* Read all tokens */
	while(xml.readNextStartElement()){

		if(xml.name() == "mark"){
			attributes.clear();
			attributes = xml.attributes();
			/* Check token value */
			if(attributes.hasAttribute("value")){
				QString tmpValue;
				int tmpToken;
				tmpValue = attributes.value("value").toString();
				tmpToken = tmpValue.toInt(&ok, 10);

				if(ok){
					/* append token to list of tokens in last palce */
					pPlaces->addToken(*pPlaces->placeList.last(), tmpToken);
					xml.skipCurrentElement();
				}
			}
		}
	}
	return TRUE;
}

/**
 * Read transition
 */
bool ReadXml::readTransition(){

	QXmlStreamAttributes attributes = xml.attributes();

	/* Read transition ID */
	if(attributes.hasAttribute("id")) {
		tmpStr = attributes.value("id").toString();

		/* Add transition with appropriate ID to transition list */
		pTrans->addTrans(tmpStr);
	}
	else return FALSE;

	/* Read transition delay */
	if(attributes.hasAttribute("delay")) {
		tmpStr = attributes.value("delay").toString();

		pTrans->setDelay(*pTrans->transList.last(), tmpStr.toInt(&ok, 10));
	}
	else return FALSE;


	/* Read conditions and operations */
	while(xml.readNextStartElement()){
		QString condition;
		QString op1, op2, rel;
		op1.clear();
		op2.clear();
		rel.clear();
		condition.clear();

		int flag = 0;
		int condSize;

		if(xml.name() == "condition"){
			attributes.clear();
			attributes = xml.attributes();

			condition = xml.readElementText();
			condSize = condition.size();

			for(int i = 0; i < condSize; i++){
				if(flag == 0){	//op1
					if(condition[0].isLetter() || condition[0].isDigit()){	//digit
						op1.append(condition[0]);
						condition.remove(0, 1);
					}
					else{	//comparison string
						rel.append(condition[0]);
						condition.remove(0, 1);
						flag = 1;
					}
				}
				else{	//op2
					if(condition[0].isLetter() || condition[0].isDigit()){
						op2.append(condition[0]);
						condition.remove(0, 1);
					}
					else{
						rel.append(condition[0]);
						condition.remove(0, 1);
					}

				}
			}

			pTrans->addCond(*pTrans->transList.last(), op1, op2, rel);

		}
		else if(xml.name() == "operation"){
			tmpStr = xml.readElementText();
			pTrans->addOper(*pTrans->transList.last(), tmpStr);
		}
		else{
			xml.skipCurrentElement();
		}
	}
	return TRUE;
}

/**
 * Read all arcs and connect transitions to places
 */
bool ReadXml::readArc(){
	QString weight;
	QChar var;

	QXmlStreamAttributes attributes = xml.attributes();
	if(attributes.hasAttribute("from")) {

		/* Read all attributes */
		tmpStr = attributes.value("from").toString();
		tmpStr2 = attributes.value("to").toString();
		if(!attributes.value("var").toString().isEmpty())
			var = attributes.value("var").toString().at(0);
		else{
			pWriteXml->sendNAck(pTcpSocket);
			return FALSE;
		}

		weight = attributes.value("weight").toString();

		/* Arc from place to transition is added to input transition list */
		if(pPlaces->getPlace(tmpStr)){
			/* *transition, *place, variable, weight */
			pTrans->connInputPlace(pTrans->getTrans(tmpStr2),
					pPlaces->getPlace(tmpStr), var, weight);

		}
		/* Arc from transition to place is added to output transition list*/
		else if(pTrans->getTrans(tmpStr)){
			/* *transition, *place, variable, weight */
			pTrans->connOutputPlace(pTrans->getTrans(tmpStr),
					pPlaces->getPlace(tmpStr2), var, weight);
		}

	}

	xml.skipCurrentElement();
	return TRUE;

}

/**
 * Read user details from xml file
 */
void ReadXml::readUser(){
	QString username, password;
	qDebug() << "reading user";
	/* Read username and password*/
	QXmlStreamAttributes attributes = xml.attributes();
	username = attributes.value("username").toString();
	password = attributes.value("password").toString();

	pUsers->addRcvUser(username, password);

	bool authOk;
	authOk = pUsers->authUser(pUsers->receivedUsername, pUsers->receivedPassword);
	if(authOk)
		pWriteXml->sendAck(pTcpSocket);
	else
		pWriteXml->sendNAck(pTcpSocket);

}

/**
 * Read GetList xml file - user requested
 * list of networks
 */
void ReadXml::readGetList(){
	status = sGETLIST;
	pWriteXml->sendAck(pTcpSocket);
}

/**
 * Read getNet xml file - user requested
 * network details
 */
void ReadXml::readGetNet(){
	status = sGETNET;
	/* Read network name and version */
	QXmlStreamAttributes attributes = xml.attributes();
	lastName = attributes.value("name").toString();
	lastVersion = attributes.value("version").toString();
	pWriteXml->sendAck(pTcpSocket);

}

/**
 * Read saveNet xml - user requested to save net
 * on the server
 */
void ReadXml::readSaveNet(){
	status = sSAVENET;
	pWriteXml->sendAck(pTcpSocket);
}

/**
 * Client sent acknowledge
 */
void ReadXml::readAck(){

	if(status == sGETNET){

		//netRepository.scanNetworks();
		pWriteXml->sendNet(lastName, lastVersion, pTcpSocket);
		status = sGETNETACK;
	}
	else if(status == sGETLIST){
		pWriteXml->sendNetList(pTcpSocket);
		status = sGETLISTACK;
	}
	else if(status == sSIMSTEP){
		execSimStepUpdate();
		status = sSIMSTEPACK;
	}
	else if(status == sSIM){
		execSimRun();
		status = sSIMACK;
	}
	else if(status == sGETLISTACK){
		pWriteXml->sendAck(pTcpSocket);
		status = sACK;
	}
	else if(status == sGETNETACK){
		pWriteXml->sendAck(pTcpSocket);
		status = sACK;
	}
	else if(status == sSIMSTEPACK){
		pWriteXml->sendAck(pTcpSocket);
		status = sACK;
	}
	else if(status == sSIMACK){
		pWriteXml->sendAck(pTcpSocket);
		status = sACK;
	}
	else
		status = sIDLE;




}

/**
 * Client sent Non - acknowledge
 */
void ReadXml::readNack(){
	status = sNACK;
}

/**
 * Client requested simulation run
 */
void ReadXml::readSimRun(){
	status = sSIM;
	qDebug() << "run SIM";

	if(pTrans->simInit == FALSE ){
		pWriteXml->sendNAck(pTcpSocket);
		status = sIDLE;
	}
	else{
		pWriteXml->sendAck(pTcpSocket);
	}

}

/**
 * Client requested simulation step
 */
void ReadXml::readSimStep(){
	status = sSIMSTEP;
	qDebug() << "SIM Step";

	if(pTrans->simInit == FALSE ){
		pWriteXml->sendNAck(pTcpSocket);
		status = sIDLE;
		return;
	}
	else{
		status = sSIMSTEP;
		execSimStep();
	}

}


/**
 * Execute one step in simulation
 */
void ReadXml::execSimStep(){
	QString transID;
	int index = -1;


	QXmlStreamAttributes attributes = xml.attributes();
	transID = attributes.value("transition").toString();

	for(int i = 0; i < pTrans->transList.count(); i++){
		if(pTrans->transList.at(i)->transitionID == transID){
			index = i;
			break;
		}
	}
	if((index != -1) && (pTrans->transList.at(index)->isExecutable())){
		pTrans->transList.at(index)->executeTrans();

		lastIndex = index;

		pWriteXml->sendAck(pTcpSocket);
		pTcpSocket->flush();

	}
	else{
		pWriteXml->sendNAck(pTcpSocket);
		//qDebug() << "no such transition or not executable";
	}

}

/**
 * Write changes in tokens to the xml file
 * ad sent it to the client
 */
void ReadXml::execSimStepUpdate(){
	/* Write changed places to Xml */
	if(lastIndex != -1){
		if(pTrans->transList.at(lastIndex) != 0)
			pWriteXml->sendUpdate(pTrans->transList.at(lastIndex)->transitionID, pTcpSocket);
	}
}

/**
 * Execute simulation. All transitions are scanned until
 * no tokens are movable or simulation run time limit
 * exceeded.
 */
void ReadXml::execSimRun(){
	int tmpRand;
	double counter = 0;

	while(true){
		QTime now = QTime::currentTime();
		qsrand(now.msec());
		tmpRand = (qrand() % (pTrans->transList.count()));

		/* check if transition is executable */
		if(pTrans->transList.at(tmpRand)->isExecutable()){
			pTrans->transList.at(tmpRand)->executeTrans();

		}
		/* simulation run time exceeded */
		if(++counter > 10000){
			break;
		}
	}

	/* send update to client */
	pWriteXml->sendRunFinalUpdate(pTcpSocket);
}

/**
 * Client started simulation
 */
void ReadXml::readSimStart(){
	qDebug() << "SIM Start";
	status = sSIMSTART;

	if(pTrans->simInit == false){
		/* simulation is initialized */
		pTrans->simInit = true;
		pWriteXml->sendAck(pTcpSocket);
	}
	else{
		pWriteXml->sendNAck(pTcpSocket);
	}
}

/**
 * Client wants to save net on the server
 * Network is saved into /networks directory
 */
void ReadXml::saveNet(){
	status = sSAVENET;
	qDebug() << "savenet file path:" << QString("/networks/"+name+"-"+version+".xml");

	QDir::setCurrent("./networks");
	QFile fileOut(name+"-"+version+".xml");
	if (!fileOut.open(QFile::WriteOnly | QFile::Text)){
			qDebug() << "cant open net file for writing" << endl;
			pWriteXml->sendNAck(pTcpSocket);
			return;
	}
	QByteArray netArray;
	netArray.append(xmlDoc);

	fileOut.write(netArray);
	pWriteXml->sendAck(pTcpSocket);
	QDir::setCurrent("..");
}


/**
 * Add username and password to database
 * @param username name of the user
 * @param password user password
 */
void UserAuth::addUser(QString username, QString password){
	users.insert(username, password);
}

/**
 * Add username and password from XML to DB
 * @param username name of the user
 * @param password user password
 */
void UserAuth::addRcvUser(QString username, QString password){
	receivedUsername = username;
	receivedPassword = password;
}

/**
 * Authenticate user - check username and password
 * @param username name of the user
 * @param password user password
 */
bool UserAuth::authUser(QString username, QString password){

	iter = users.find(username);

	if(iter == users.end()){
		return FALSE;	//username not found
	}
	else{
		if(iter.value() != password){
			return FALSE;	//wrong password
		}
	}
	return TRUE;	//auth ok
}
