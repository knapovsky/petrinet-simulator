/**
 * @file server.h
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

#ifndef SERVER_H_
#define SERVER_H_

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include "simulation.h"
#include "xmlParse.h"

#define TEST_PORT 44444


/**
 * Main Server Class
 * Starts functions and classes
 */
class Server: public QTcpServer{
	Q_OBJECT

private:

public:
	/**
	 * Server class constructor
	 * Start listening, set ip addresses, ports
	 * @param *parent pointer to parent object
	 */
	Server(QObject *parent = 0);

protected:
	/**
	 * Start new thread for new connection
	 * @param socketDescriptor descriptor of the socket
	 */
	void incomingConnection(int socketDescriptor);

};

/**
 * Class represents thread - every simulation has
 * its own thread.
 */
class SimThread : public QThread{
	Q_OBJECT
private:
	quint16 blockSize;
public:
	SimThread(int socketDescriptor, QObject *parent);
	QTcpSocket *tcpSocket;
	ReadXml *readXml;
	WriteXml *writeXml;
	void run();

signals:
	void error(QTcpSocket::SocketError socketError);

private slots:
	/**
	* Read new socket data
	*/
	void processReadyRead();

private:

	int socketDescriptor;
};

#endif /* SERVER_H_ */
