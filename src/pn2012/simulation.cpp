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
 * Implementace tridy reprezentujici simulaci a vse potrebne pro jeji zobrazeni.
 *
 */

#include "simulation.h"
#include "networklayer.h"
#include "simulationnetworklayer.h"


Simulation::Simulation(QString fileName_in, Settings *modeObjectIn, QString simFile)
{

    // Inicializace
    simulationLayout = NULL;
    graphView = NULL;
    splitter = NULL;
    scene = NULL;
    properties = NULL;
    modeObject = NULL;
    simData = NULL;
    simNetLayer = NULL;
    modeObject = modeObjectIn;

    // Vytvoreni objektu pro data simulace
    simData = new SimulationData(fileName_in, simFile, modeObject, this, properties);

    //connect(simData, SIGNAL(parsingError()), this, SLOT(removeSimulation()));

    /* Vytvoreni widgetu simulace */
    if(simData){

        createSimulationWidgets();

        // Vytvori sitovou vrstvu, ktera bude pouzita pro simulaci
        simNetLayer = new SimulationNetworkLayer(modeObject, simData);
    }
}

void Simulation::updateScene(){

    this->scene->update();
}

QString Simulation::getFilename()
{
    return simData->getFilename();
}

void Simulation::itemSelected(DiagramItem *item)
{
    if(modeObject->displayProperties){
    //qDebug() << "Simulation item selected pointer" << item;
        properties->setItem(item);
    }
}

void Simulation::runSimulation()
{
    // Kontrola, zda nejsou simulacni data v rozporu
    if(simData->checkSyntaxBeforeSimulation()){

        /* Tady se simulace odesle na server a tam se provede */
        simNetLayer->sendSimStart(tRUN);
    }
    else{
        return;
    }

    //qDebug() << "Running simulation on server";
    return;
}

void Simulation::nextStepSimulation()
{
    /* Simulace uz je na serveru a pouze se posle prikaz na provedeni dalsiho kroku */
    /* Ze serveru prijde aktualizace stavu */
    simNetLayer->sendSimStart(tSTEP);

    //qDebug() << "Next step in simulation requested";
    return;
}

int Simulation::saveSimulationLocally()
{
    /* Tento slot se vola po spusteni akce save */
    //qDebug() << "Saving simulation file locally";

    if(!(simData->fileName).isEmpty()){

        simData->generateXML(simData->fileName);
    }
    else{

        saveAsSimulationLocally();
    }
    return 1;
}

int Simulation::saveAsSimulationLocally()
{
    /* Soubor se simulaci se lokalne ulozi do souboru specifikovaneho QStringem - cestou */
    //qDebug() << "Saving simulation file as ... locally";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ":",
                               tr("XML (*.xml)"));

    //qDebug() << "Filename: " << fileName;
    if(!fileName.isEmpty()){

        // Vygenerovani XML souboru
        simData->generateXML(fileName);
    }

    return 1;
}

int Simulation::saveSimulationToServer()
{
    /* Simulace se odesle na server s prikazem na ulozeni - nesimuluje se */
    //qDebug() << "Saving simulation file to server";
    return 1;
}

int Simulation::restartSimulation()
{
    /* Restart simulace */
    // Znovunacteni simulacniho souboru
    simData->reloadSimulation();
    simNetLayer->disconnectFromServer();

    // Zobrazeni prvky ve scene
    QList<DiagramPlace*>::iterator diagramPlaceIterator;
    for(diagramPlaceIterator = simData->diagramPlaces.begin(); diagramPlaceIterator != simData->diagramPlaces.end(); diagramPlaceIterator++){

        scene->addItem(*diagramPlaceIterator);
    }
    QList<DiagramTransition*>::iterator diagramTransitonIterator;
    for(diagramTransitonIterator = simData->diagramTransitions.begin(); diagramTransitonIterator != simData->diagramTransitions.end(); diagramTransitonIterator++){

        scene->addItem(*diagramTransitonIterator);
    }
    QList<DiagramArc *>::iterator diagramArcIterator;
    for(diagramArcIterator = simData->diagramArcs.begin(); diagramArcIterator != simData->diagramArcs.end(); diagramArcIterator++){

        scene->addItem(*diagramArcIterator);
    }

    //qDebug() << "Restarting simulation";
    return 1;
}

void Simulation::removeSimulation()
{
    //qDebug() << "Emitting simulationError";
    emit simulationError();
}

DiagramScene *Simulation::getScene()
{
    return scene;
}

SimulationData *Simulation::getSimData()
{
    return simData;
}

void Simulation::resizeProps()
{
    QList<int> sizes;
    sizes.append(500);
    sizes.append(1);
    this->splitter->setSizes(sizes);

    if(properties){
        properties->hide();
    }
}

void Simulation::createSimulationWidgets()
{
    /* Layout simulace */
    simulationLayout = new QVBoxLayout;
    properties = new DiagramProperties(simData, modeObject);
    connect(properties, SIGNAL(itemRemoved()), this, SLOT(resizeProps()));
    connect(properties, SIGNAL(itemUpdated()), this, SLOT(updateScene()));

    //properties->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //qDebug() << "simdata: " << simData << "modeObject:" <<  modeObject << "this:" <<  this << "props" <<  properties;

    scene = new DiagramScene(simData, modeObject, this, properties);
    scene->setSceneRect(0, 0, modeObject->sceneWidth, modeObject->sceneHeight);
    connect(scene, SIGNAL(mousePressed()), this, SLOT(resizeProps()));

    splitter = new QSplitter(Qt::Vertical, this);
    simulationLayout->setSizeConstraint(QLayout::SetMaximumSize);

    /* Napojeni pozadavku na smazani hran */
    connect(simData, SIGNAL(removeItem()), scene, SLOT(removeDiagramItem()));
    /* Napojeni pozadavku na smazani itemu */
    connect(simData, SIGNAL(removeArcs()), scene, SLOT(removeDiagramArcs()));
    /* Napojeni pozadavku na vlozeni itemu */
    connect(simData, SIGNAL(insertItem()), scene, SLOT(insertDiagramItem()));

    graphView = new QGraphicsView(this);
    //scene = new DiagramScene();
    graphView->setScene(scene);

    QList<DiagramPlace*>::iterator diagramPlaceIterator;
    for(diagramPlaceIterator = simData->diagramPlaces.begin(); diagramPlaceIterator != simData->diagramPlaces.end(); diagramPlaceIterator++){
        scene->addItem(*diagramPlaceIterator);
    }
    QList<DiagramTransition*>::iterator diagramTransitonIterator;
    for(diagramTransitonIterator = simData->diagramTransitions.begin(); diagramTransitonIterator != simData->diagramTransitions.end(); diagramTransitonIterator++){
        scene->addItem(*diagramTransitonIterator);
    }
    QList<DiagramArc *>::iterator diagramArcIterator;
    for(diagramArcIterator = simData->diagramArcs.begin(); diagramArcIterator != simData->diagramArcs.end(); diagramArcIterator++){
        scene->addItem(*diagramArcIterator);
    }

    /* Zapnuti sledovani pohybu mysi */
    QList<int> sizes;
    sizes.append(500);
    sizes.append(1);
    graphView->setMouseTracking(true);
    splitter->addWidget(graphView);
    splitter->addWidget(properties);
    splitter->setSizes(sizes);

    simulationLayout->addWidget(splitter);

    this->setLayout(simulationLayout);
}

void Simulation::disconnectFromServer(){

    this->simNetLayer->disconnectFromServer();
}
