/**
 * @file netrepository.h
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Parsing networks from network directory
 */

#ifndef NETREPOSITORY_H_
#define NETREPOSITORY_H_

#include <QDir>
#include <QMap>
#include <QString>
#include <QDomDocument>

/**
 * Repository for networks in network directory.
 */
class NetRepository{
private:
	/* Vytvoreni dokumentu */
	QDomDocument document;

	QDir myDir;
	QStringList list;

public:
	/**
	 * Scan networks available in network directory
	 * and generate QDomDocument.
	 * @return list of networks
	 */
	QDomDocument scanNetworks();


};


#endif /* NETREPOSITORY_H_ */
