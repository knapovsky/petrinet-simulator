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
 * Implementace tridy reprezentujici grafickou scenu, do ktere se vykresluji
 * mista, prechody a hrany diagramu simulace.
 *
 */

#include "diagramscene.h"

DiagramScene::DiagramScene(SimulationData* simDataIn,
                           Settings *modeObjectPointer_in,
                           Simulation *simulationPointerIn,
                           DiagramProperties *diagramPropertiesIn)
{
    // Nastaveni promennych objektu
    simData = simDataIn;
    diagramProperties = diagramPropertiesIn;
    modeObjectPointer = modeObjectPointer_in;
    simulationPointer = simulationPointerIn;
    line = NULL;
    pressed = false;

    // Nastaveni pozadi sceny
    if(modeObjectPointer != NULL){
        this->setBackgroundBrush(modeObjectPointer->pattern);
    }
}

void DiagramScene::removeDiagramItem()
{
    // Odstrani prvek sceny a aktualizuje ji
    this->removeItem(simData->toRemoveItem);
    simData->toRemoveItem = NULL;
    this->update();
}

void DiagramScene::removeDiagramArcs()
{
    // Odstranuje prvky, ktere jsou v seznamu prvku k odstraneni
    QList<DiagramArc*>::iterator iter;
    for(iter = simData->toRemoveDiagramArcs.begin(); iter != simData->toRemoveDiagramArcs.end(); iter++){
        // Odstrani ho
        this->removeItem(*iter);
    }

    // Vycisti se seznam prvku k odstraneni
    simData->toRemoveDiagramArcs.clear();

    // Aktualizace sceny
    this->update();
}

void DiagramScene::insertDiagramItem(){

    // Vlozi prvek, ktery je odkazovan v simData
    this->addItem(simData->toInsertItem);

    // Prvek je automaticky vybran
    modeObjectPointer->selectedItem = simData->toInsertItem;

    // Je vlozen, ukazatel v simData se zneplatni
    simData->toInsertItem = NULL;

    // Aktualizace sceny
    update();
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    // Ruzne chovani toho v jakem modu se aktualne nachazime
    // Vkladani mist
    if(modeObjectPointer->getCurrentMode() == ModePlace){

        // Vytvoreni promennych, ktere budou pouzity pro vytvoreni noveho mista
        QList<int> val;
        qreal width = 100;
        qreal height = 100;

        // Nazev musi byt unikatni
        QString tempString = modeObjectPointer->defaultPlaceName;
        tempString.append(QString("%1").arg(simData->placeCounter));
        simData->placeCounter++;
        /* Osetreni unikatnosti noveho prvku */
        QList<DiagramPlace *>::iterator iter;
        for(iter = simData->diagramPlaces.begin(); iter != simData->diagramPlaces.end(); iter++){

            // Prochazeji se mista v simulaci a porovna se jejich nazev s nazvem, ktery hodlame
            // nastavit nove vlozenemu mistu
            if((*iter)->properties->id == tempString){
                tempString = modeObjectPointer->defaultPlaceName;
                tempString.append(QString("%1").arg(simData->placeCounter));
                iter = simData->diagramPlaces.begin();
                simData->placeCounter++;
            }
        }

        // Vytvoreni noveho mista
        DiagramPlace *newPlace = new DiagramPlace(tempString,
                                                  val,
                                                  event->scenePos().x() - width/2,
                                                  event->scenePos().y() - height/2,
                                                  width,
                                                  height,
                                                  modeObjectPointer->defaultColor,
                                                  0,
                                                  "",
                                                  modeObjectPointer, simData);

        // Misto je automaticky vybrano
        modeObjectPointer->setSelectedItem(newPlace);

        // Prida se do seznamu mist
        simData->diagramPlaces.append(newPlace);

        // Prida se do sceny
        this->addItem(newPlace);
    }

    // Mod vkladani prechodu
    else if(modeObjectPointer->getCurrentMode() == ModeTransition){

        // Prazdny seznam podminek
        QList<QString> conds;

        // Prazdny seznam operaci
        QList<QString> ops;

        // Sirka a vyska
        qreal width = 50;
        qreal height = 100;

        // Vytvoreni unikatniho identifikatoru noveho prechodu
        // Ziskani nazvu z globalniho nastaveni
        QString tempString = modeObjectPointer->defaultTransitionName;

        // Pripojeni aktualniho pocitadla prechodu
        tempString.append(QString("%1").arg(simData->transitionCounter));
        simData->transitionCounter++;

        /* Osetreni unikatnosti noveho prvku */
        QList<DiagramTransition *>::iterator iter;
        for(iter = simData->diagramTransitions.begin(); iter != simData->diagramTransitions.end(); iter++){

            // Prochazeni hran a hledani stejneho ID
            if((*iter)->properties->id == tempString){

                // Pokud je nalezeno, zvysi se pocitadlo prechodu a vytvori se novy nazev,
                // ktery je opet vyhledavan na shodu mezi prechody do doby,
                // dokud neni nazev unikatni
                tempString = modeObjectPointer->defaultTransitionName;
                tempString.append(QString("%1").arg(simData->transitionCounter));
                iter = simData->diagramTransitions.begin();
                simData->transitionCounter++;
            }
        }

        // Vytvoreni noveho prechodu
        DiagramTransition *newTransition = new DiagramTransition(tempString,
                                                                 conds,
                                                                 ops,
                                                                 event->scenePos().x() - width/2,
                                                                 event->scenePos().y() - height/2,
                                                                 50,
                                                                 100,
                                                                 modeObjectPointer->defaultColor,
                                                                 0,
                                                                 "",
                                                                 modeObjectPointer,
                                                                 simData);

        // Pridani do seznamu prechodu
        simData->diagramTransitions.append(newTransition);

        // Nastaveni noveho prechodu jako vybraneho
        modeObjectPointer->setSelectedItem(newTransition);

        // Pridani prechodu do sceny
        this->addItem(newTransition);
    }

    // Mod hran
    else if(modeObjectPointer->getCurrentMode() == ModeArc){

        // Pri stisku se vytvori nova primka, ktera bude nasledne aktualizova
        // pri pohybu mysi se stisknutym tlacitkem
        line = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));

        // Nastaveni pera pro vykreslovani primky
        line->setPen(QPen(Qt::black, 5));

        // Pridani primky do sceny
        addItem(line);
    }

    // Jiny mod - nic se neprovadi
    else{;}

    // Aktualizace sceny
    update();

    // Pokud je zvolen mod "kurzor", event se deleguje dal
    if(modeObjectPointer->getCurrentMode() == ModePointer){

        this->pressed = true;
        emit mousePressed();
        QGraphicsScene::mousePressEvent(event);
    }

}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Vypis aktualni pozice kurzoru ve scene do status baru
    QString position = QString("Position: %1 %2").arg(event->scenePos().x()).arg(event->scenePos().y());
    modeObjectPointer->getMainWindowPointer()->statusBar()->showMessage(position);

    // Pri zvolenem modu vkladani hran se aktualizuje koncovy bod primky
    if(modeObjectPointer->getCurrentMode() == ModeArc && line != 0){

        // Aktualiza a prirazeni nove primky
        QLineF newLine(line->line().p1(), event->scenePos());
        line->setLine(newLine);
    }

    this->update();

    /*if(this->pressed){
        emit mousePressed();
    }*/

    // Delegace eventu
    QGraphicsScene::mouseMoveEvent(event);
}

void DiagramScene::keyPressEvent(QKeyEvent *event)
{

    // Odchytava se stisknute tlacitko DELETE, kterym se
    // odstranuji prvky sceny
    if(event->key() == Qt::Key_Delete){

        //qDebug() << "scene trying to remove";
        simData->removeSelectedItem();
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

    //qDebug() << "Releasing Mouse in graphics scene";
    //qDebug() << "\n";

    this->pressed = false;

    // Pri zvolenem modu vkladani primek a pusteni tlacitka mysi
    // se kontroluje, zda koncovy bod nelezi uvnitr nektereho
    // prvku - kontroly nejsou provadeny ciste na bounding rectangle,
    // ale primo na pozici uvnitr vykresleneho prvku
    // Je-li tedy prvek elipsa, bude se kontrolovat pozice uvnitr elipsy
    if(line != 0 && modeObjectPointer->getCurrentMode() == ModeArc){

        // true = place->trans, false = trans->place
        bool direction = false;

        DiagramItem *placePointer = NULL;
        DiagramItem *place;

        // Prochazeji se vsechna mista simulace a kontroluje se kolize
        foreach(place, simData->diagramPlaces){
            if(placePointer == NULL){
               if(place->isInsideBoundingRect(line->line().p1())){

                   // Nalezeno misto, ktere koliduje s pocatecnim bodem primky
                   //qDebug() << "start collides with place: " << place->properties->id;
                   placePointer = place;
                   direction = true;
               }
               if(place->isInsideBoundingRect(line->line().p2())){

                     // Nalezeno misto, ktere koliduje s koncovym bodem primky
                     //qDebug() << "end collides with place: " << place->properties->id;
                     placePointer = place;
                     direction = false;
               }
             }
        }

        /* Direction uz je nastavena - nemusi se nastavovat znovu */
        DiagramItem *transitionPointer = NULL;
        DiagramItem *transition;

        // Prochazeji se prechody a kontroluje se zda nekoliduji s body primky
        foreach(transition, simData->diagramTransitions){
            if(transitionPointer == NULL){
                if(transition->isInsideBoundingRect(line->line().p1())){

                     // Pocatecni bod primky koliduje s touto hranou
                     //qDebug() << "start collides with transition: " << transition->properties->id;
                     transitionPointer = transition;
               }
               if(transition->isInsideBoundingRect(line->line().p2())){

                     // Koncovy bod primky koliduje s touto hranou
                     //qDebug() << "end collides with transition: " << transition->properties->id;
                     transitionPointer = transition;
               }
             }
        }

        /* K tomu, aby se vytvorila hrana musi byt nalezeno misto a prechod, jinak se prvky nepropoji */
        if(placePointer != NULL && transitionPointer != NULL){

            /* Vytvoreni hrany */
            //qDebug() << "vytvarim hranu, direction = " << direction;
            /* Vytvoreni unikatniho id hrany */
            QString arcID = modeObjectPointer->defaultArcName;
            arcID.append(QString("%1").arg(simData->arcCounter));
            simData->arcCounter++;

            // Prochazeni hran a kontrola, zda jiz neni vytvorena hrana s timto nazvem
            QList<DiagramArc *>::iterator iter;
            for(iter = simData->diagramArcs.begin(); iter != simData->diagramArcs.end(); iter++){

                if((*iter)->properties->id == arcID){

                    // Pokud neni nazev hrany unikatni, vytvori se novy inkrementaci pocitadla
                    // a nazev se kontroluje porovnavanim znovu
                    arcID = modeObjectPointer->defaultArcName;
                    arcID.append(QString("%1").arg(simData->arcCounter));
                    iter = simData->diagramArcs.begin();
                    simData->arcCounter++;
                }
            }

            // Vytvoreni nove hrany
            DiagramArc *newArc = new DiagramArc(arcID, "", (DiagramPlace*)placePointer, (DiagramTransition*)transitionPointer, direction, modeObjectPointer->defaultWeight, modeObjectPointer->defaultArcColor, modeObjectPointer->defaultArcIndex, "", modeObjectPointer, simData);

            /* Pridani nove hrany do seznamu hran */
            simData->diagramArcs.append(newArc);

            /* Pridani hrany do sceny */
            this->addItem(newArc);
        }

        // Pri pusteni tlacitka mysi se odstrani primka, ktera se pri
        // tahu mysi se stlacenym tlacitkem vykreslovala
        removeItem(line);

        // Aktualizace sceny
        update();
    }

    // Aktualizace sceny
    this->update();

    /* Znovuvyslani signalu, aby ho odchytil DiagramItem */
    QGraphicsScene::mouseReleaseEvent(event);

}
