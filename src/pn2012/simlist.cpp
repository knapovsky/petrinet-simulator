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
 * Implementace tridy, ktera uchovava seznam aktualne otevrenych simulaci, umoznuje
 * s nimi pracovat, vytvaret nove a odstranovat je.
 *
 */

#include "simlist.h"

SimList::SimList(Settings *modeObjectIn)
{
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentSimulation(int)));
    settings = modeObjectIn;
    currentSimulation = addSimulation("", settings, settings->defaultNewSimulation);
    settings->simList = this;
    this->setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

Simulation* SimList::addSimulation(QString fileName, Settings *modeObjectIn, QString simFile)
{
    //qDebug() << "Adding simulation " << fileName << " Settings " << modeObjectIn << " simfile:\n" << simFile;

    QDomDocument document;

    if(!fileName.isEmpty()){
       // qDebug() << "opening file: " << fileName;
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                //qDebug() << "Failed to open file in addSimulation";
                //return -1;
        }
        //qDebug() << "File opened\n";
        // Nahrani souboru
        if(!document.setContent(&file))
        {
            //qDebug() << "Failed to load document!";
            //return -1;
        }
        // Uzavreni souboru
        file.close();
    }
    else if(!simFile.isEmpty()){
        //qDebug() << "opening file from string";
        if(!document.setContent(simFile)){
            qDebug() << "Failed to load document from server due to wrong XML format";
        }
    }
    else{
        qDebug() << "SimList: No file or SimString entered";
    }

    QDomElement root = document.firstChildElement();
    QDomNodeList heads = root.elementsByTagName("head");
    QDomNode head = heads.at(0);
    QDomElement headEl = head.toElement();
    QDomNodeList names = headEl.elementsByTagName("name");
    QDomNode nameN = names.at(0);
    QDomElement nameEl = nameN.toElement();
    QString name = nameEl.text();

    if(name.isEmpty()) name = "New";

    if(!simFile.isEmpty()){
        newSim = new Simulation(fileName, modeObjectIn, simFile);
    }
    else{
        newSim = new Simulation(fileName, modeObjectIn, "");
    }
    //connect(newSim, SIGNAL(simulationError()), this, SLOT(simCreationError()));
    if(this->newSim){
        //qDebug() << "NEWSIM:" << this->newSim;
        this->simulations.insert(this->simulations.size(), this->newSim);
        // Tady se precte jmeno souboru
        this->addTab(this->newSim, name);
        this->changeCurrentSimulation(newSim);

        return this->newSim;
    }
    else{
        return NULL;
    }


}

void SimList::itemSelected(DiagramItem *item)
{
    currentSimulation->itemSelected(item);
    //qDebug() << "simlist item selected";
}

void SimList::changeCurrentSimulation(int i)
{
    currentSimulation = simulations.at(i);
    this->setCurrentWidget(currentSimulation);
}

void SimList::changeCurrentSimulation(Simulation *sim){
    currentSimulation = sim;
    this->setCurrentWidget(sim);
}

void SimList::simCreationError()
{
    //qDebug() << "Simulation creation error";
    //qDebug() << "Count Tab: " << this->count();
    //this->removeTab(this->count() - 1);
    //delete simulations.at(this->count() -1);
}

void SimList::closeTab(int i){

    if(this->count() == 1){
        return;
    }
    this->removeTab(i);
    this->simulations.removeAt(i);

}

void SimList::disconnectFromServer(){

    QList<Simulation*>::iterator simulationListIter;
    for(simulationListIter = this->simulations.begin(); simulationListIter != this->simulations.end(); simulationListIter++){
        (*simulationListIter)->disconnectFromServer();
    }
}
