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
 * Implementace tridy globalniho nastaveni programu.
 *
 */

#include "settings.h"
#include "diagramitem.cpp"

Settings::Settings(Mode modeIn)
{

    // Pocatecni nastaveni

    currentMode = modeIn;
    selectedPen.setBrush(Qt::black);
    selectedPen.setStyle(Qt::DashLine);
    selectedPen.setWidth(3);
    notSelectedPen.setBrush(Qt::black);
    notSelectedPen.setStyle(Qt::SolidLine);
    notSelectedPen.setWidth(1);
    this->selectedItem = NULL;
    defaultWeight = 1;
    defaultArcIndex = -1;
    displayProperties = true;

    // Nastaveni barev
    defaultColor = Qt::white;
    defaultArcColor = Qt::black;
    colors << Qt::white << Qt::black << Qt::red << Qt::blue << Qt::yellow << Qt::gray << Qt::darkGray << Qt::magenta << Qt::darkMagenta << Qt::cyan;
    names << tr("white") << tr("black") << tr("red") << tr("blue")
          << tr("yellow") << tr("grey") << tr("dark grey") << tr("magenta") << tr("dark magenta") << tr("cyan");

    // Nastaveni jmen nove vytvorenych prvku
    defaultArcName = "newArc";
    defaultPlaceName = "newPlace";
    defaultTransitionName = "newTrans";

    // Pozadi sceny
    pattern = Qt::Dense7Pattern;

    // Rozmery sceny
    sceneWidth = 800;
    sceneHeight = 600;

    // Timeout
    statusBarTimeout = 2000;

    // Validatory
    weightValidator = new QRegExp("^[1-9]\\d{1, 2}$");
    idValidator = new QRegExp("^[A-Za-z0-9_]{1,10}$");
    dimensionValidator = new QRegExp("^[1-9]\\d{1, 2}$");
    indexValidator = new QRegExp("^-?[0-9]\\d{1, 2}$");

    // Server informace
    ip = "";
    port = "44444";
    username = "testuser";
    password = "password";

    /* Pokud o otevreni souboru s nastavenim */
    if(loadSettingsFromXML("config.xml")){
        //qDebug() << "nastaveni nacteno z configu";
        this->configFile = "config.xml";
    }
    else{
        //qDebug() << "Nastaveni nebylo nacteno";
    }

    // Vytvoreni obsahu nove vytvorene simulace
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    defaultNewSimulation = QString("<PN type=\"net\">\n<head>\n<author>john doe</author>\n<version>0.1</version>\n<date>%1</date>\n<name>new</name>\n<description>new network</description>\n</head>\n</PN>").arg(date);
}

Mode Settings::getCurrentMode()
{
    return currentMode;
}

void Settings::saveSettingsToXML()
{
    /* Vytvoreni dokumentu */
    QDomDocument document;

    /* Vytvoreni root elementu */
    QDomElement root = document.createElement("PN");

    /* Pridani root elem. do dokumentu */
    root.setAttribute("type", "settings");
    document.appendChild(root);

    /* Pridani dalsich elementu */
    /* Head */
    QDomElement head = document.createElement("head");
    root.appendChild(head);

    /* Body */
    QDomElement body = document.createElement("body");
    root.appendChild(body);

    QDomElement defaultColorE = document.createElement("defaultcolor");
    QDomElement defaultArcColorE = document.createElement("defaultarccolor");
    QDomElement defaultPlaceNameE = document.createElement("defaultplacename");
    QDomElement defaultTransitionNameE = document.createElement("defaulttransitionname");
    QDomElement defaultArcNameE = document.createElement("defaultarcname");
    QDomElement defaultWeightE = document.createElement("defaultweight");
    QDomElement defaultArcIndexE = document.createElement("defaultarcindex");
    QDomElement sceneWidthE = document.createElement("scenewidth");
    QDomElement sceneHeightE = document.createElement("sceneheight");
    //QDomElement patternE = document.createElement("scenepattern");
    QDomElement usernameE = document.createElement("username");
    QDomElement passwordE = document.createElement("password");
    QDomElement ipE = document.createElement("ip");
    QDomElement portE = document.createElement("port");


    QDomText defaultColorString = document.createTextNode(this->defaultColor.name());
    QDomText defaultArcColorString = document.createTextNode(this->defaultArcColor.name());
    QDomText defaultPlaceNameString = document.createTextNode(this->defaultPlaceName);
    QDomText defaultTransitionNameString = document.createTextNode(this->defaultTransitionName);
    QDomText defaultArcNameString = document.createTextNode(this->defaultArcName);
    QDomText defaultWeightString = document.createTextNode(QString("%1").arg(this->defaultWeight));
    QDomText defaultArcIndexString = document.createTextNode(QString("%1").arg(this->defaultArcIndex));
    QDomText sceneWidthString = document.createTextNode(QString("%1").arg(this->sceneWidth));
    QDomText sceneHeightString = document.createTextNode(QString("%1").arg(this->sceneHeight));
    QDomText usernameString = document.createTextNode(this->username);
    QDomText passwordString = document.createTextNode(this->password);
    QDomText ipString = document.createTextNode(this->ip);
    QDomText portString = document.createTextNode(this->port);

    //QDomText patternString = document.createTextNode(this->pattern.)

    defaultColorE.appendChild(defaultColorString);
    defaultArcColorE.appendChild(defaultArcColorString);
    defaultPlaceNameE.appendChild(defaultPlaceNameString);
    defaultTransitionNameE.appendChild(defaultTransitionNameString);
    defaultArcNameE.appendChild(defaultArcNameString);
    defaultWeightE.appendChild(defaultWeightString);
    defaultArcIndexE.appendChild(defaultArcIndexString);
    sceneWidthE.appendChild(sceneWidthString);
    sceneHeightE.appendChild(sceneHeightString);
    usernameE.appendChild(usernameString);
    passwordE.appendChild(passwordString);
    ipE.appendChild(ipString);
    portE.appendChild(portString);


    body.appendChild(defaultColorE);
    body.appendChild(defaultArcColorE);
    body.appendChild(defaultPlaceNameE);
    body.appendChild(defaultTransitionNameE);
    body.appendChild(defaultArcNameE);
    body.appendChild(defaultWeightE);
    body.appendChild(defaultArcIndexE);
    body.appendChild(sceneWidthE);
    body.appendChild(sceneHeightE);
    body.appendChild(usernameE);
    body.appendChild(passwordE);
    body.appendChild(ipE);
    body.appendChild(portE);

    /* Zapis site do souboru */
    QString currentDirPath =  QDir::currentPath();
    QFile file("config.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //qDebug() << "Failed to open file for writting";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        qDebug() << document.toString();
        file.close();
        qDebug() << "Finished saving config";
    }
}

bool Settings::loadSettingsFromXML(QString path)
{

    QDomDocument document;

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            //qDebug() << "Failed to open config file";
            return false;
    }
    //qDebug() << "File opened\n";

    // Nahrani souboru
    if(!document.setContent(&file))
    {
        //qDebug() << "Failed to load document!";
        return false;
    }
    // Uzavreni souboru
    file.close();

    QDomElement root = document.firstChildElement();

    QDomNodeList defaultColorList = root.elementsByTagName("defaultcolor");
    QDomNodeList defaultArcColorList = root.elementsByTagName("defaultarccolor");
    QDomNodeList defaultPlaceNameList = root.elementsByTagName("defaultplacename");
    QDomNodeList defaultTransitionNameList = root.elementsByTagName("defaulttransitionname");
    QDomNodeList defaultArcNameList = root.elementsByTagName("defaultarcname");
    QDomNodeList defaultWeightList = root.elementsByTagName("defaultweight");
    QDomNodeList defaultArcIndexList = root.elementsByTagName("defaultarcindex");
    QDomNodeList sceneWidthList = root.elementsByTagName("scenewidth");
    QDomNodeList sceneHeightList = root.elementsByTagName("sceneheight");
    QDomNodeList usernameList = root.elementsByTagName("username");
    QDomNodeList passwordList = root.elementsByTagName("password");
    QDomNodeList ipList = root.elementsByTagName("ip");
    QDomNodeList portList = root.elementsByTagName("port");


    QDomNode defaultColorN = defaultColorList.at(0);
    QDomNode defaultArcColorN = defaultArcColorList.at(0);
    QDomNode defaultPlaceNameN = defaultPlaceNameList.at(0);
    QDomNode defaultTransitionNameN = defaultTransitionNameList.at(0);
    QDomNode defaultArcNameN = defaultArcNameList.at(0);
    QDomNode defaultWeightN = defaultWeightList.at(0);
    QDomNode defaultArcIndexN = defaultArcIndexList.at(0);
    QDomNode sceneWidthN = sceneWidthList.at(0);
    QDomNode sceneHeightN = sceneHeightList.at(0);
    QDomNode usernameN = usernameList.at(0);
    QDomNode passwordN = passwordList.at(0);
    QDomNode ipN = ipList.at(0);
    QDomNode portN = portList.at(0);

    /* Nacteni hodnot do tohoto objektu */
    this->defaultColor.setNamedColor(defaultColorN.toElement().text());
    this->defaultArcColor.setNamedColor(defaultArcColorN.toElement().text());
    this->defaultPlaceName = defaultPlaceNameN.toElement().text();
    this->defaultTransitionName = defaultTransitionNameN.toElement().text();
    this->defaultArcName = defaultArcNameN.toElement().text();
    this->defaultWeight = defaultWeightN.toElement().text().toInt();
    this->defaultArcIndex = defaultArcIndexN.toElement().text().toInt();
    this->sceneWidth = sceneWidthN.toElement().text().toInt();
    this->sceneHeight = sceneHeightN.toElement().text().toInt();
    this->username = usernameN.toElement().text();
    this->password = passwordN.toElement().text();
    this->ip = ipN.toElement().text();
    this->port = portN.toElement().text();


    // Nastaveni cesty k souboru s nastavenim
    this->configFile = path;

    return true;
}

void Settings::setCurrentMode(Mode modeIn)
{
    currentMode = modeIn;
}

void Settings::setMainWindowPointer(QMainWindow *mainWindowPointerIn)
{
    mainWindowPointer = mainWindowPointerIn;
}

void Settings::setSelectedItem(DiagramItem *selectedItemIn)
{
    this->selectedItem = selectedItemIn;

    // Vyslani signalu o vyberu
    emit itemSelected(this->selectedItem);
}

void Settings::updateItem(DiagramItem *item)
{
    emit itemSelected(item);
}

void Settings::showAlert(QString message)
{
    QMessageBox alert(mainWindowPointer);
    alert.setWindowTitle("Alert");
    alert.setIcon(QMessageBox::Warning);
    alert.setText(message);
    alert.exec();
}

void Settings::showInfo(QString message)
{
    QMessageBox info(mainWindowPointer);
    info.setWindowTitle("Info");
    info.setIcon(QMessageBox::Information);
    info.setText(message);
    info.exec();
}

QMainWindow *Settings::getMainWindowPointer()
{
    return mainWindowPointer;
}

void Settings::setConfigFile(QString configFileIn)
{
    configFile = configFileIn;
    loadSettingsFromXML(configFileIn);
}

void Settings::loadDefaultSettings(){

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

    defaultColor = Qt::white;
    defaultArcColor = Qt::black;
    this->defaultPlaceName = "newPlace";
    this->defaultTransitionName = "newTransition";
    this->defaultArcName = "newArc";
    this->defaultWeight = 1;
    this->defaultArcIndex = 20;
    this->sceneWidth = 800;
    this->sceneHeight = 600;
    this->username = "";
    this->password = "";
    this->ip = ipAddress;
    this->port = "44444";
}
