/**
 * @file netrepository.cpp
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

#include "netrepository.h"

#include <QDebug>

/**
 * Scan networks available in network directory
 * and generate QDomDocument.
 * @return list of networks
 */
QDomDocument NetRepository::scanNetworks(){
/*	if(!myDir.cd("networks")){
		if(!myDir.cd("examples"))
			qDebug() << "Cannot find networks directory";
	}*/

	QDir::setCurrent("./networks");

	/* open history file */
	QFile fileHist("history.myxml");
	if(!fileHist.open(QFile::ReadOnly | QFile::Text)){
		qDebug() << "cant open history file" << endl;

		fileHist.open(QFile::ReadWrite | QFile::Text);

		/* Vytvoreni root elementu */
		QDomElement rootH = document.createElement("PN");
		/* Pridani root elem. do dokumentu */
		rootH.setAttribute("type", "history");
		document.appendChild(rootH);
		/* Body */
		QDomElement bodyH = document.createElement("body");
		rootH.appendChild(bodyH);

		QByteArray histArray;
		histArray.append(document.toString());

		fileHist.write(histArray);
		fileHist.close();

		document.clear();

		if(!fileHist.open(QFile::ReadOnly | QFile::Text))
			qDebug() << "really cant open history file" << endl;
	}

	else{

		/* create element for history file */
		QDomDocument documentHIST;
		documentHIST.setContent(&fileHist);

		document.clear();
		list = myDir.entryList(QStringList("*.xml"));

		/* Vytvoreni root elementu */
		QDomElement root = document.createElement("PN");
		/* Pridani root elem. do dokumentu */
		root.setAttribute("type", "list");
		document.appendChild(root);

		/* Pridani dalsich elementu */
		/* Head */
		QDomElement head = document.createElement("head");
		root.appendChild(head);

		/* Body */
		QDomElement body = document.createElement("body");
		root.appendChild(body);

		QStringList::iterator iter;
		for(iter = list.begin(); iter != list.end(); iter++){
		    QDomDocument tempDocument;

		    QFile file(*iter);
		    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		    {
		        qDebug() << "Failed to open file in parseXML";

		    }
		    // Nahrani souboru
		    QString errorMsg;
		    if(!tempDocument.setContent(&file, &errorMsg))
		    {
		        qDebug() << "Failed to load document!" << errorMsg;

		    }
		    // Uzavreni souboru
		    file.close();

		    QDomElement tempRoot = tempDocument.firstChildElement();

		    QDomNodeList authorList = tempRoot.elementsByTagName("author");
		    QDomNodeList versionList = tempRoot.elementsByTagName("version");
		    QDomNodeList dateList = tempRoot.elementsByTagName("date");
		    QDomNodeList nameList = tempRoot.elementsByTagName("name");

		    QDomNode authorN = authorList.at(0);
		    QDomNode versionN = versionList.at(0);
		    QDomNode dateN = dateList.at(0);
		    QDomNode nameN = nameList.at(0);

		    QString author = authorN.toElement().text();
		    QString version = versionN.toElement().text();
		    QString date = dateN.toElement().text();
		    QString name = nameN.toElement().text();

		    QDomElement net = document.createElement("net");
		    net.setAttribute("name", name);
		    net.setAttribute("version", version);
		    net.setAttribute("author", author);
		    net.setAttribute("date", date);

		    /* add history */

		    QDomElement tempHist = documentHIST.firstChildElement();
		    QDomNodeList historyNodes = tempHist.elementsByTagName("history");

		    QDomAttr attributes;
		    for(int i = 0; i < historyNodes.count(); i++){
		    	QDomElement history;

		    	/* append history */
				if(historyNodes.at(i).toElement().hasAttribute("name")){

				   	if((historyNodes.at(i).toElement().attributeNode("name").value() == name) &&
				   			(historyNodes.at(i).toElement().attributeNode("version").value() == version)){

						history = document.createElement("history");
						history.setAttribute("by", historyNodes.at(i).toElement().attributeNode("by").value());
						history.setAttribute("date", historyNodes.at(i).toElement().attributeNode("date").value());
						history.setAttribute("time", historyNodes.at(i).toElement().attributeNode("time").value());

						net.appendChild(history);
					}
		    	}
		    }

		    body.appendChild(net);
		}
		documentHIST.clear();
		fileHist.close();
	}

	QDir::setCurrent("..");
	//myDir.cdUp();
	return document;
}
