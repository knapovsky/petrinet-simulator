/**
 * @file main.cpp
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Main class of the Qt server.
 */

#include <QtCore>
#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
	
    return a.exec();
}

