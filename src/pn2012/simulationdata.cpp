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
 * Implementace tridy pro uchovani simulacnich dat a praci s nimi.
 *
 */

#include "simulationdata.h"

SimulationData::SimulationData(QString fileNameIn, QString simFileIn, Settings *&settingsIn, Simulation *simulationIn, DiagramProperties *propertiesIn)
{

    // Inicializace
    placeCounter = transitionCounter = arcCounter = 0;
    fileName = fileNameIn;
    settings = settingsIn;
    simulation = simulationIn;
    properties = propertiesIn;
    simFile = simFileIn;

    // Docasne misto a prechod, ktery ma byt zkopirovan
    QList<int> i;
    QList<QString> temp;
    copyPlace = new DiagramPlace("", i, 0, 0, 0, 0, Qt::black, 0, "", settings, this);
    copyTransition = new DiagramTransition("", temp, temp, 0, 0, 0, 0, Qt::black, 0, "", settings, this);

    /* Pozor, parser predava ukazatele - musi byt az na konci */
    // Nacteni dat ze vstupniho souboru
    parseXML();
}

QString SimulationData::getFilename()
{
    return fileName;
}

void SimulationData::parsePlaceElements(QDomElement root)
{
    QDomNodeList items = root.elementsByTagName("place");

    //qDebug() << "Total items = " << items.count();

    for(int i = 0; i < items.count(); i++)
    {
       QDomNode itemNode = items.at(i);
       //convert to element
       if(itemNode.isElement())
       {
           QDomElement elem = itemNode.toElement();
           QString id = elem.attribute("id");
           qreal x = elem.attribute("x").toFloat();
           qreal y = elem.attribute("y").toFloat();
           qreal width = elem.attribute("width").toFloat();
           qreal height = elem.attribute("height").toFloat();
           QString desc = elem.attribute("desc");
           QString color = elem.attribute("color");
           int index = elem.attribute("index").toInt();

           /* Testovaci vypis
           qDebug() << "Config: " << " x: " << x << " y: " << y
                    << " width: " << width << " height: " << height
                    << " desc: " << desc << " color: " << color << "\n";
            */
           /* Precteni oznaceni mista */
           QDomNodeList marks = elem.elementsByTagName("mark");
           QList<int> markList;
           for(int h = 0; h < marks.count(); h++){

               QDomNode mk = marks.at(h);
               if(mk.isElement()){
                   QDomElement el = mk.toElement();
                   markList.append(el.attribute("value").toInt());
                   //qDebug() << "Mark: " << el.attribute("value").toInt() << "\n";
               }
           }

           /** Prirazeni vsech nactenych promennych do objektu **/
           /* Vytvoreni noveho mista */
           /* Osetreni unikatnosti ID */
           DiagramPlace *iPlace;
           foreach(iPlace, diagramPlaces){

               if(iPlace->properties->id == id){

                   QMessageBox alert;
                   //emit parsingError();
                   alert.setText(QString("ID \"%1\" is not unique. Name has been automatically adjusted.").arg(id));
                   alert.exec();
                   id = settings->defaultPlaceName;
                   id.append(QString("%1").arg(placeCounter));
               }
           }

           DiagramPlace *tempPlace = new DiagramPlace(id,
                                                      markList,
                                                      x,
                                                      y,
                                                      width,
                                                      height,
                                                      QColor(color),
                                                      index,
                                                      desc,
                                                      settings,
                                                      this
                                                      );

           /* Pridani mista do seznamu mist */
           diagramPlaces.append(tempPlace);

       }
    }

}

void SimulationData::parseTransitionElements(QDomElement root)
{
    QDomNodeList items = root.elementsByTagName("transition");

     //qDebug() << "Total items = " << items.count();

     for(int i = 0; i < items.count(); i++)
     {
        QDomNode itemNode = items.at(i);

        //convert to element
        if(itemNode.isElement())
        {
            QDomElement elem = itemNode.toElement();
            QString id = elem.attribute("id");
            //qint32 delay = elem.attribute("delay").toInt();
            qreal x = elem.attribute("x").toFloat();
            qreal y = elem.attribute("y").toFloat();
            qreal width = elem.attribute("width").toFloat();
            qreal height = elem.attribute("height").toFloat();
            QString desc = elem.attribute("desc");
            QString color = elem.attribute("color");
            int index = elem.attribute("index").toInt();

            /*
            qDebug() << "Config: " << " x: " << x << " y: " << y
                     << " width: " << width << " height: " << height
                     << " desc: " << desc << " color: " << color
                     << " delay: " << delay <<"\n";
            */
            QDomNodeList conditions = elem.elementsByTagName("condition");
            QList<QString> conditionsList;
            for(int h = 0; h < conditions.count(); h++){

                QDomNode cond = conditions.at(h);
                if(cond.isElement()){

                    QDomElement el = cond.toElement();
                    conditionsList.append(el.text());
                    //qDebug() << "Condition: " << el.text() << "\n";
                }
            }

            QDomNodeList operations = elem.elementsByTagName("operation");
            QList<QString> operationsList;
            for(int h = 0; h < operations.count(); h++){

                QDomNode op = operations.at(h);
                if(op.isElement()){

                    QDomElement o = op.toElement();
                    operationsList.append(o.text());
                    //qDebug() << "Operation: " << o.text() << "\n";
                }
            }

            /* Osetreni unikatnosti ID */
            DiagramTransition *iTrans;
            foreach(iTrans, diagramTransitions){

                if(iTrans->properties->id == id){

                    QMessageBox alert;
                    //emit parsingError();
                    alert.setText(QString("ID \"%1\" is not unique. Name has been automatically adjusted.").arg(id));
                    alert.exec();
                    id = settings->defaultTransitionName;
                    id.append(QString("%1").arg(transitionCounter));
                }
            }

            /* Pridani nacteneho prechodu do objektu */
            DiagramTransition *tempTransition = new DiagramTransition(id,
                                                                      conditionsList,
                                                                      operationsList,
                                                                      x,
                                                                      y,
                                                                      width,
                                                                      height,
                                                                      QColor(color),
                                                                      index,
                                                                      desc,
                                                                      settings,
                                                                      this
                                                                      );

            /* Pridani noveho prechodu do seznamu prechodu */
            diagramTransitions.append(tempTransition);
        }
     }
}

void SimulationData::parseArcElements(QDomElement root)
{
    QDomNodeList items = root.elementsByTagName("arc");

     //qDebug() << "Total items = " << items.count();

     for(int i = 0; i < items.count(); i++)
     {
        QDomNode itemNode = items.at(i);

        //convert to element
        if(itemNode.isElement())
        {
            QDomElement elem = itemNode.toElement();
            QString id = elem.attribute("id");
            QString desc = elem.attribute("desc");
            QString color = elem.attribute("color");
            QString from = elem.attribute("from");
            QString to = elem.attribute("to");
            QString var = elem.attribute("var");
            qint32 weight = elem.attribute("weight").toInt();
            int index = elem.attribute("index").toInt();
            /*
            qDebug() << "Config: " << " desc: " << desc << " color: " << color
                     << " id: " << id << " from: " << from << " to: " << to
                     << " var: " << var << " weight: " << weight << "\n";
            */
            /* Nalezeni prvku uvedeneho ve from */
            DiagramPlace *diagramPlacePointer = NULL;
            DiagramTransition *diagramTransitionPointer = NULL;
            // true = place->trans, false = trans->place
            bool direction = false;

            QList<DiagramPlace*>::iterator diagramPlacesIterator;
            for(diagramPlacesIterator = diagramPlaces.begin(); diagramPlacesIterator != diagramPlaces.end(); diagramPlacesIterator++){

                if((*diagramPlacesIterator)->getID() == from){

                    if(diagramPlacePointer == NULL){
                        diagramPlacePointer = *diagramPlacesIterator;
                        direction = true;
                    }
                    else{

                        // byly nalezeny dve mista se stejnym nazvem
                        qDebug() << "Error: More places called " << from << " found. ID must be unique";
                    }
                }
                if((*diagramPlacesIterator)->getID() == to){

                    if(diagramPlacePointer == NULL){
                        diagramPlacePointer = *diagramPlacesIterator;
                        direction = false;
                    }
                    else{

                        // byly nalezeny dve mista se stejnym nazvem
                        qDebug() << "Error: More places called " << to << " found. ID must be unique";
                    }
                }
            }

            QList<DiagramTransition*>::iterator diagramTransitionIterator;
            for(diagramTransitionIterator = diagramTransitions.begin(); diagramTransitionIterator != diagramTransitions.end(); diagramTransitionIterator++){
                if((*diagramTransitionIterator)->getID() == from){

                    if(diagramTransitionPointer == NULL){

                        if(direction == false){

                           diagramTransitionPointer = *diagramTransitionIterator;
                        }
                        else{

                            // Prechod z prechodu do prechodu :D
                            qDebug() << "Error: Arc from transition to transition";
                        }

                    }
                    else{

                        // Odkazovany prvek obsazen vicekrat
                        qDebug() << "Error: More transitions called " << from << " found. ID must be unique";
                    }
                }
                if((*diagramTransitionIterator)->getID() == to){

                    if(diagramTransitionPointer == NULL){
                        if(direction == true){
                            // Vse jak ma byt ;)
                           diagramTransitionPointer = *diagramTransitionIterator;
                        }
                        else{

                            // Prechod z prechodu do prechodu :D
                            qDebug() << "Error: Arc from transition to transition";
                        }

                    }
                    else{

                        // Odkazovany prvek obsazen vicekrat
                        qDebug() << "Error: More transitions called " << to << " found. ID must be unique";
                    }
                }
            }

            /* Vytvoreni noveho spoje */
            DiagramArc *tempArc = new DiagramArc(id,
                                                 var,
                                                 diagramPlacePointer,
                                                 diagramTransitionPointer,
                                                 direction,
                                                 weight,
                                                 QColor(color),
                                                 index,
                                                 desc,
                                                 settings,
                                                 this
                                                 );

            /* Pridani do seznamu spoju */
            diagramArcs.append(tempArc);
        }
     }
}



void SimulationData::parseHead(QDomElement root){

    QDomNodeList authorList = root.elementsByTagName("author");
    QDomNodeList versionList = root.elementsByTagName("version");
    QDomNodeList dateList = root.elementsByTagName("date");
    QDomNodeList descriptionList = root.elementsByTagName("description");
    QDomNodeList nameList = root.elementsByTagName("name");

    QDomNode authorN = authorList.at(0);
    QDomNode versionN = versionList.at(0);
    QDomNode dateN = dateList.at(0);
    QDomNode descriptionN = descriptionList.at(0);
    QDomNode nameN = nameList.at(0);

    this->author = authorN.toElement().text();
    this->version = versionN.toElement().text();
    this->date = dateN.toElement().text();
    this->description = descriptionN.toElement().text();
    this->name = nameN.toElement().text();
}

void SimulationData::parseXML()
{
    QDomDocument document;

    //qDebug() << "Parse XML " << simFile << " Filename: " << fileName;

    if(simFile.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                qDebug() << "Failed to open file in parseXML";
                //return -1;
        }
        //qDebug() << "File opened\n";
        // Nahrani souboru
        if(!document.setContent(&file))
        {
            qDebug() << "Failed to load document!";
            //return -1;
        }
        // Uzavreni souboru
        file.close();
    }
    else if(!simFile.isEmpty()){
        if(!document.setContent(simFile)){
            qDebug() << "Failed to load document from string";
        }
    }
    else{
        qDebug() << "No document to parse";
    }

    QDomElement root = document.firstChildElement();

    /* Parsovani hran musi byt posledni - pro semantickou kontrolu XML vyhledava ve vytvorenych objektech mist a prechodu */
    parseHead(root);
    parsePlaceElements(root);
    parseTransitionElements(root);
    parseArcElements(root);

    //qDebug() << "Succesfully parsed";
}

void SimulationData::parseUpdate(QString update)
{

    QDomDocument document;
    if(!document.setContent(update))
    {
        qDebug() << "Failed to load document!";
        return;
    }

    QDomElement root = document.firstChildElement();

    QDomNodeList items = root.elementsByTagName("place");

    //qDebug() << "Total items in update = " << items.count();

     for(int i = 0; i < items.count(); i++)
     {
        QDomNode itemNode = items.at(i);

        //convert to element
        if(itemNode.isElement())
        {
            QDomElement elem = itemNode.toElement();
            /* ID mista */
            QString id = elem.attribute("id");

            QDomNodeList marks = elem.elementsByTagName("mark");
            QList<int> values;
            for(int h = 0; h < marks.count(); h++){
                QDomNode mark = marks.at(h);
                if(mark.isElement()){
                    QDomElement markEl = mark.toElement();
                    values.append(markEl.attribute("value").toInt());
                }
            }

            /* Nahrazeni obsahu aktualne nacitaneho mista */
            QList<DiagramPlace*>::iterator placeIter;
            for(placeIter = diagramPlaces.begin(); placeIter != diagramPlaces.end(); placeIter++){

                if((*placeIter)->properties->id == id){

                    (*placeIter)->values = values;

                    /* Aktualizace mista */
                    (*placeIter)->prepareGeometryChangePublic();
                    (*placeIter)->update();
                }
            }
         }
     }

}

QString SimulationData::colorDecoder(QColor colorIn){

    QList<QColor>::iterator colorIter;
    int i = 0;
    for(colorIter = settings->colors.begin(); colorIter != settings->colors.end(); colorIter++){

        if(*colorIter == colorIn){

            return settings->names.at(i);
        }
        i++;
    }
    return "";
}

void SimulationData::cutSelectedItem(){

    /* Neni vybrana zadna polozka */
    if(settings->selectedItem == NULL) return;

    /* Polozka musi byt v teto simulaci ne v jine */
    if(!isItemHere(settings->selectedItem)) return;

    //qDebug() << "cutting item id: " << settings->selectedItem->properties->id;

    /* Pokud neni polozka, ktera je ulozena pro kopirovani zobrazena, pak se odstrani z pameti */
    if(!isItemHere(copyPlace) && copyPlace != NULL){
        delete copyPlace;
        copyPlace = NULL;
    }

    if(!isItemHere(copyTransition) && copyTransition != NULL){
        delete copyTransition;
        copyTransition = NULL;
    }

    DiagramItemProperty *props = settings->selectedItem->properties;
    if(settings->selectedItem->type == Place){
        DiagramPlace* place = (DiagramPlace*)settings->selectedItem;
        copyPlace = new DiagramPlace(props->id, place->values, props->x, props->y, props->width, props->height, props->color, props->index, props->description, settings, this);
        settings->copiedItem = copyPlace;
    }
    if(settings->selectedItem->type == Transition){
        DiagramTransition *trans = (DiagramTransition*)settings->selectedItem;
        copyTransition = new DiagramTransition(props->id, trans->conditions, trans->operations, props->x, props->y, props->width, props->height, props->color, props->index, props->description, settings, this);
        settings->copiedItem = copyTransition;
    }

    if(settings->selectedItem != NULL){
        removeSelectedItem();
    }
}

void SimulationData::pasteItem()
{

    if(settings->copiedItem == NULL) return;
    if(settings->copiedItem->type == Place){
        this->diagramPlaces.append((DiagramPlace*)settings->copiedItem);
    }
    if(settings->copiedItem->type == Transition){
        this->diagramTransitions.append((DiagramTransition*)settings->copiedItem);
    }

    toInsertItem = settings->copiedItem;
    if(toInsertItem != NULL){
        emit insertItem();
    }

}

void SimulationData::updateNetDate()
{
    this->date = QDate::currentDate().toString("dd.MM.yyyy");
}

void SimulationData::updateNetVersion()
{
    float version = this->version.toFloat();
    version += 0.1;
    this->version = QString("%1").arg(version);
}

void SimulationData::reloadSimulation()
{
    //qDebug() << "Reloading simulation file";

    QString authorBackup = author;
    QString dateBackup = date;
    QString nameBackup = name;
    QString descriptionBackup = description;
    QString versionBackup = version;

    // Vyprazdneni seznamu
    // Seznam propojeni
    QList<DiagramArc*>::iterator arcIter;
    for(arcIter = diagramArcs.begin(); arcIter != diagramArcs.end(); arcIter++){
        delete *arcIter;
    }
    diagramArcs.clear();
    // Seznam mist
    QList<DiagramPlace*>::iterator placeIter;
    for(placeIter = diagramPlaces.begin(); placeIter != diagramPlaces.end(); placeIter++){
        delete *placeIter;
    }
    diagramPlaces.clear();
    // Seznam prechodu
    QList<DiagramTransition*>::iterator transIter;
    for(transIter = diagramTransitions.begin(); transIter != diagramTransitions.end(); transIter++){
        delete *transIter;
    }
    diagramTransitions.clear();

    //delete properties;
    this->parseXML();

    author = authorBackup;
    date = dateBackup;
    name = nameBackup;
    description = descriptionBackup;
    version = versionBackup;
}

void SimulationData::saveCurrentNet(){

    this->simFile = this->getNet().toString();
}

bool SimulationData::isItemHere(DiagramItem *item)
{
    if(item == NULL) return false;

    /* Zjisti typ prvku a projde seznam, kde jsou prvky tohoto typu obsazeny */
    ItemType type = item->type;

    if(type == Place){
        DiagramPlace *place = (DiagramPlace*) item;
        QList<DiagramPlace*>::iterator placeIter;
        for(placeIter = diagramPlaces.begin(); placeIter != diagramPlaces.end(); placeIter++){
            if(*placeIter == place){
                return true;
            }
        }
        return false;
    }
    else if(type == Transition){
        DiagramTransition *place = (DiagramTransition*) item;
        QList<DiagramTransition*>::iterator transIter;
        for(transIter = diagramTransitions.begin(); transIter != diagramTransitions.end(); transIter++){
            if(*transIter == place){
                return true;
            }
        }
        return false;
    }
    else if(type == Arc){
        DiagramArc *place = (DiagramArc*) item;
        QList<DiagramArc*>::iterator arcIter;
        for(arcIter = diagramArcs.begin(); arcIter != diagramArcs.end(); arcIter++){
            if(*arcIter == place){
                return true;
            }
        }
        return false;
    }
    else{
        return false;
    }
    return false;
}

QString SimulationData::getNewPlaceName()
{
    QString tempString = settings->defaultPlaceName;
    tempString.append(QString("%1").arg(this->placeCounter));
    this->placeCounter++;

    /* Osetreni unikatnosti noveho prvku */
    QList<DiagramPlace *>::iterator iter;
    for(iter = this->diagramPlaces.begin(); iter != this->diagramPlaces.end(); iter++){
        if((*iter)->properties->id == tempString){
            tempString = settings->defaultPlaceName;
            tempString.append(QString("%1").arg(this->placeCounter));
            iter = this->diagramPlaces.begin();
            this->placeCounter++;
        }
    }

    return tempString;
}

QString SimulationData::getNewTransitionName()
{
    QString tempString = settings->defaultTransitionName;
    tempString.append(QString("%1").arg(this->transitionCounter));
    this->transitionCounter++;

    /* Osetreni unikatnosti noveho prvku */
    QList<DiagramTransition *>::iterator iter;
    for(iter = this->diagramTransitions.begin(); iter != this->diagramTransitions.end(); iter++){
        //qDebug() << "transition id: " << (*iter)->properties->id;
        if((*iter)->properties->id == tempString){
            tempString = settings->defaultTransitionName;
            tempString.append(QString("%1").arg(this->transitionCounter));
            iter = this->diagramTransitions.begin();
            this->transitionCounter++;
        }
    }

    return tempString;
}

QString SimulationData::getNewArcName()
{
    QString arcID = settings->defaultArcName;
    arcID.append(QString("%1").arg(this->arcCounter));
    this->arcCounter++;

    QList<DiagramArc *>::iterator iter;
    for(iter = this->diagramArcs.begin(); iter != this->diagramArcs.end(); iter++){
        if((*iter)->properties->id == arcID){
            arcID = settings->defaultArcName;
            arcID.append(QString("%1").arg(this->arcCounter));
            iter = this->diagramArcs.begin();
            this->arcCounter++;
        }
    }

    return arcID;
}

void SimulationData::copySelectedItem()
{
    if(settings->selectedItem == NULL) return;

    //qDebug() << "copying item id: " << settings->selectedItem->properties->id;


    // Odstraneni predchozi zkopirovane polozky

    if(copyPlace != NULL){
        delete copyPlace;
        copyPlace = NULL;
    }
    if(copyTransition != NULL){
        delete copyTransition;
        copyTransition = NULL;
    }

    // Vytvoreni nove polozky se stejnymi vlastnostmi jako ma polozka oznacena
    DiagramItemProperty *props = settings->selectedItem->properties;
    if(settings->selectedItem->type == Place){
        DiagramPlace* place = (DiagramPlace*)settings->selectedItem;
        copyPlace = new DiagramPlace(props->id, place->values, props->x, props->y, props->width, props->height, props->color, props->index, props->description, settings, this);
        settings->copiedItem = copyPlace;
    }
    if(settings->selectedItem->type == Transition){
        DiagramTransition *trans = (DiagramTransition*)settings->selectedItem;
        copyTransition = new DiagramTransition(props->id, trans->conditions, trans->operations, props->x, props->y, props->width, props->height, props->color, props->index, props->description, settings, this);
        settings->copiedItem = copyTransition;
    }
}

bool SimulationData::listContains(QList<int> list, int item){

    QList<int>::iterator i;
    for(i = list.begin(); i != list.end(); i++){
        if(*i == item) return true;
    }
    return false;
}

void SimulationData::removeArcContaining(QString id){

   // qDebug() << "Removing id: " << id;
    QList<DiagramArc*>::iterator arcIter;
    int i = 0;
    QList<int> indexes;

    /* Pruchod hranami a vyhledavani, zda nejsou pripojeny k prvku s danym ID */

    for(arcIter = this->diagramArcs.begin(); arcIter != this->diagramArcs.end(); arcIter++){
        if((*arcIter != NULL)){
           // qDebug() << "Snuffing Arc: " << (*arcIter)->properties->id;

            if(((*arcIter)->getPlace() != NULL) && ((*arcIter)->getTransition() != NULL)){

                if((*arcIter)->getPlace()->properties->id == id || (*arcIter)->getTransition()->properties->id == id){
                    indexes.append(i);
                }
            }
            else{
               // qDebug() << "How so? Arc: " << (*arcIter)->properties->id;
            }
        }
        i++;
    }
    //qDebug() << "Removing arc indexed by : " << indexes;

    QList<DiagramArc*> newList;
    QList<DiagramArc*> removeList;
    int index = 0;
    for(arcIter = this->diagramArcs.begin(); arcIter != this->diagramArcs.end(); arcIter++){

        if(!listContains(indexes, index)){
            //qDebug() << "Appendig index " << index;
            newList.append(*arcIter);
        }
        else{
            removeList.append(*arcIter);
        }
        index++;
    }

    /* Prepis nove vytvoreneho seznamu */
    this->diagramArcs = newList;

    toRemoveDiagramArcs = removeList;
    if(!toRemoveDiagramArcs.isEmpty()){
        emit removeArcs();
    }
}

void SimulationData::removeSelectedItem()
{
   // qDebug() << "deleting selected item";
    if(settings->selectedItem == NULL) return;

    /* Odstraneni ze sceny */
    /* Odstaneni ze seznamu */
    if(settings->selectedItem->properties == NULL) {
    //    qDebug() << "HOW SO?";
    }
    QString id = settings->selectedItem->properties->id;
    QList<DiagramPlace*>::iterator placeIter;
    QList<DiagramTransition*>::iterator transIter;
    QList<DiagramArc*>::iterator arcIter;
    int i = 0;

    removeArcContaining(id);

    toRemoveItem = settings->selectedItem;
    if(toRemoveItem != NULL){
        emit removeItem();
    }


    /* Dle typu prvku se prochazi seznamy daneho typu */

    switch(settings->selectedItem->type){

        case(Place):
            for(placeIter = this->diagramPlaces.begin(); placeIter != this->diagramPlaces.end(); placeIter++){
                if((*placeIter != NULL)){
                    if(*placeIter == settings->selectedItem){
                        this->diagramPlaces.removeAt(i);
                        settings->setSelectedItem(NULL);
                        return;
                    }
                }
                i++;
            }
            break;
        case (Transition):
            for(transIter = this->diagramTransitions.begin(); transIter != this->diagramTransitions.end(); transIter++){
                if((*transIter) == settings->selectedItem){
                    this->diagramTransitions.removeAt(i);
                    settings->setSelectedItem(NULL);
                    return;
                }
                i++;
            }
            break;
        case (Arc):
            for(arcIter = this->diagramArcs.begin(); arcIter != this->diagramArcs.end(); arcIter++){
                if((*arcIter) == settings->selectedItem){
                    this->diagramArcs.removeAt(i);
                    settings->setSelectedItem(NULL);
                    return;

                    //delete *arcIter;
                }
                i++;
            }
            break;
        default:
            break;

    }

}

bool SimulationData::checkSyntaxBeforeSimulation(){

    /* Musi byt obsazeno misto, prechod a hrana, ktera musi mit pridelenou promennou */

    QList<DiagramArc*>::iterator arcIter;
    for(arcIter = diagramArcs.begin(); arcIter != diagramArcs.end(); arcIter++){
        if((*arcIter)->getVariable() == ""){

            QString message;
            message = "No variable on arc id: ";
            message.append((*arcIter)->properties->id);
            settings->showAlert(message);

            return false;
        }
    }

    if(diagramPlaces.length() <= 0){
        settings->showAlert("No places. Simulation is meaningless");
        return false;
    }

    if(diagramTransitions.length() <= 0){
        settings->showAlert("No transition. Simulation is meaningless");
        return false;
    }

    if(diagramArcs.length() <= 0){
        settings->showAlert("No arc. Simulation is meaningless");
        return false;
    }

    return true;
}

void SimulationData::generateXML(QString fileLocation){

    /* Vytvoreni dokumentu */
    QDomDocument document;
    /* Vytvoreni root elementu */
    QDomElement root = document.createElement("PN");
    /* Pridani root elem. do dokumentu */
    root.setAttribute("type", "net");
    document.appendChild(root);

    /* Pridani dalsich elementu */
    /* Head */
    QDomElement head = document.createElement("head");
    root.appendChild(head);

    /* Prvky hlavicky */
    QDomElement author = document.createElement("author");
    QDomElement version = document.createElement("version");
    QDomElement name = document.createElement("name");
    QDomElement date = document.createElement("date");
    QDomElement description = document.createElement("description");
    QDomText authorString = document.createTextNode(this->author);
    QDomText versionString = document.createTextNode(this->version);
    QDomText nameString = document.createTextNode(this->name);
    QDomText dataString = document.createTextNode(this->date);
    QDomText descriptionString = document.createTextNode(this->description);
    author.appendChild(authorString);
    version.appendChild(versionString);
    name.appendChild(nameString);
    date.appendChild(dataString);
    description.appendChild(descriptionString);

    head.appendChild(name);
    head.appendChild(author);
    head.appendChild(version);
    head.appendChild(date);
    head.appendChild(description);

    /* Body */
    QDomElement body = document.createElement("body");
    root.appendChild(body);

    /* Prvky tela */
    /* Mista */
    QList<DiagramPlace*>::iterator placeIter;
    for(placeIter = diagramPlaces.begin(); placeIter != diagramPlaces.end(); placeIter++){
        QDomElement place = document.createElement("place");
        place.setAttribute("id", (*placeIter)->properties->id);
        place.setAttribute("x", (*placeIter)->properties->x);
        place.setAttribute("y", (*placeIter)->properties->y);
        place.setAttribute("width", (*placeIter)->properties->width);
        place.setAttribute("height", (*placeIter)->properties->height);
        place.setAttribute("desc", (*placeIter)->properties->description);
        place.setAttribute("color", colorDecoder((*placeIter)->properties->color));
        place.setAttribute("index", (*placeIter)->properties->index);

        QList<int>::iterator marks;
        for(marks = (*placeIter)->values.begin(); marks != (*placeIter)->values.end(); marks++){
            QDomElement mark = document.createElement("mark");
            mark.setAttribute("value", *marks);

            /* Pridani ohodnoceni do dokumentu */
            place.appendChild(mark);
        }

        /* Pridani mista do dokumentu */
        body.appendChild(place);
    }
    /* Prechody */
    QList<DiagramTransition*>::iterator transIter;
    for(transIter = diagramTransitions.begin(); transIter != diagramTransitions.end(); transIter++){
        QDomElement transition = document.createElement("transition");
        transition.setAttribute("id", (*transIter)->properties->id);
        transition.setAttribute("delay", (*transIter)->delay);
        transition.setAttribute("x", (*transIter)->properties->x);
        transition.setAttribute("y", (*transIter)->properties->y);
        transition.setAttribute("width", (*transIter)->properties->width);
        transition.setAttribute("height", (*transIter)->properties->height);
        transition.setAttribute("desc", (*transIter)->properties->description);
        transition.setAttribute("color", colorDecoder((*transIter)->properties->color));
        transition.setAttribute("index", (*transIter)->properties->index);

        /* Pridani podminek do tohoto prechodu */
        QList<QString>::iterator condIter;
        for(condIter = (*transIter)->conditions.begin(); condIter != (*transIter)->conditions.end(); condIter++){
            QDomElement condition = document.createElement("condition");
            QDomText condString;
            condString = document.createTextNode(*condIter);
            condition.appendChild(condString);

            /* Pridani podminky do tohoto prechodu */
            transition.appendChild(condition);
        }
        /* Pridani operaci do tohoto prechodu */
        QList<QString>::iterator operIter;
        for(operIter = (*transIter)->operations.begin(); operIter != (*transIter)->operations.end(); operIter++){
            QDomElement operation = document.createElement("operation");
            operation.setNodeValue(*operIter);

            QDomText opString;
            opString = document.createTextNode(*operIter);
            operation.appendChild(opString);

            /* Pridani operace do tohoto prechodu */
            transition.appendChild(operation);
        }

        /* Pridani prechodu do dokumentu */
        body.appendChild(transition);
    }

    /* Hrany */
    QList<DiagramArc*>::iterator arcIter;
    for(arcIter = diagramArcs.begin(); arcIter != diagramArcs.end(); arcIter++){
        QDomElement arc = document.createElement("arc");
        arc.setAttribute("id", (*arcIter)->properties->id);
        /* Place -> Transition */
        if((*arcIter)->getDirection()){
            arc.setAttribute("from", (*arcIter)->getPlace()->properties->id);
            arc.setAttribute("to", (*arcIter)->getTransition()->properties->id);
        }
        else{
            arc.setAttribute("from", (*arcIter)->getTransition()->properties->id);
            arc.setAttribute("to", (*arcIter)->getPlace()->properties->id);
        }
        arc.setAttribute("var", (*arcIter)->getVariable());
        arc.setAttribute("weight", (*arcIter)->getWeight());
        arc.setAttribute("desc", (*arcIter)->properties->description);
        arc.setAttribute("color", colorDecoder((*arcIter)->properties->color));
        arc.setAttribute("index", (*arcIter)->properties->index);

        /* Pridani hrany do dokumentu */
        body.appendChild(arc);
    }

    /* Zapis site do souboru */
    QFile file(fileLocation);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        //qDebug() << "Finished";
    }

    this->fileName = fileLocation;
}

QDomDocument SimulationData::getNet(){

    /* Vytvoreni dokumentu */
    QDomDocument document;
    /* Vytvoreni root elementu */
    QDomElement root = document.createElement("PN");
    /* Pridani root elem. do dokumentu */
    root.setAttribute("type", "net");
    document.appendChild(root);

    /* Pridani dalsich elementu */
    /* Head */
    QDomElement head = document.createElement("head");
    root.appendChild(head);

    /* Prvky hlavicky */
    QDomElement author = document.createElement("author");
    QDomElement version = document.createElement("version");
    QDomElement name = document.createElement("name");
    QDomElement date = document.createElement("date");
    QDomElement description = document.createElement("description");
    QDomText authorString = document.createTextNode(this->author);
    QDomText versionString = document.createTextNode(this->version);
    QDomText nameString = document.createTextNode(this->name);
    QDomText dataString = document.createTextNode(this->date);
    QDomText descriptionString = document.createTextNode(this->description);
    author.appendChild(authorString);
    version.appendChild(versionString);
    name.appendChild(nameString);
    date.appendChild(dataString);
    description.appendChild(descriptionString);

    head.appendChild(name);
    head.appendChild(author);
    head.appendChild(version);
    head.appendChild(date);
    head.appendChild(description);

    /* Body */
    QDomElement body = document.createElement("body");
    root.appendChild(body);

    /* Prvky tela */
    /* Mista */
    QList<DiagramPlace*>::iterator placeIter;
    for(placeIter = diagramPlaces.begin(); placeIter != diagramPlaces.end(); placeIter++){
        QDomElement place = document.createElement("place");
        place.setAttribute("id", (*placeIter)->properties->id);
        place.setAttribute("x", (*placeIter)->properties->x);
        place.setAttribute("y", (*placeIter)->properties->y);
        place.setAttribute("width", (*placeIter)->properties->width);
        place.setAttribute("height", (*placeIter)->properties->height);
        place.setAttribute("desc", (*placeIter)->properties->description);
        place.setAttribute("color", colorDecoder((*placeIter)->properties->color));
        place.setAttribute("index", (*placeIter)->properties->index);

        QList<int>::iterator marks;
        for(marks = (*placeIter)->values.begin(); marks != (*placeIter)->values.end(); marks++){
            QDomElement mark = document.createElement("mark");
            mark.setAttribute("value", *marks);

            /* Pridani ohodnoceni do dokumentu */
            place.appendChild(mark);
        }

        /* Pridani mista do dokumentu */
        body.appendChild(place);
    }
    /* Prechody */
    QList<DiagramTransition*>::iterator transIter;
    for(transIter = diagramTransitions.begin(); transIter != diagramTransitions.end(); transIter++){
        QDomElement transition = document.createElement("transition");
        transition.setAttribute("id", (*transIter)->properties->id);
        transition.setAttribute("delay", (*transIter)->delay);
        transition.setAttribute("x", (*transIter)->properties->x);
        transition.setAttribute("y", (*transIter)->properties->y);
        transition.setAttribute("width", (*transIter)->properties->width);
        transition.setAttribute("height", (*transIter)->properties->height);
        transition.setAttribute("desc", (*transIter)->properties->description);
        transition.setAttribute("color", colorDecoder((*transIter)->properties->color));
        transition.setAttribute("index", (*transIter)->properties->index);

        /* Pridani podminek do tohoto prechodu */
        QList<QString>::iterator condIter;
        for(condIter = (*transIter)->conditions.begin(); condIter != (*transIter)->conditions.end(); condIter++){
            QDomElement condition = document.createElement("condition");
            QDomText condString;
            condString = document.createTextNode(*condIter);
            condition.appendChild(condString);

            /* Pridani podminky do tohoto prechodu */
            transition.appendChild(condition);
        }
        /* Pridani operaci do tohoto prechodu */
        QList<QString>::iterator operIter;
        for(operIter = (*transIter)->operations.begin(); operIter != (*transIter)->operations.end(); operIter++){
            QDomElement operation = document.createElement("operation");
            operation.setNodeValue(*operIter);

            QDomText opString;
            opString = document.createTextNode(*operIter);
            operation.appendChild(opString);

            /* Pridani operace do tohoto prechodu */
            transition.appendChild(operation);
        }

        /* Pridani prechodu do dokumentu */
        body.appendChild(transition);
    }

    /* Hrany */
    QList<DiagramArc*>::iterator arcIter;
    for(arcIter = diagramArcs.begin(); arcIter != diagramArcs.end(); arcIter++){
        QDomElement arc = document.createElement("arc");
        arc.setAttribute("id", (*arcIter)->properties->id);
        /* Place -> Transition */
        if((*arcIter)->getDirection()){
            arc.setAttribute("from", (*arcIter)->getPlace()->properties->id);
            arc.setAttribute("to", (*arcIter)->getTransition()->properties->id);
        }
        else{
            arc.setAttribute("from", (*arcIter)->getTransition()->properties->id);
            arc.setAttribute("to", (*arcIter)->getPlace()->properties->id);
        }
        arc.setAttribute("var", (*arcIter)->getVariable());
        arc.setAttribute("weight", (*arcIter)->getWeight());
        arc.setAttribute("desc", (*arcIter)->properties->description);
        arc.setAttribute("color", colorDecoder((*arcIter)->properties->color));
        arc.setAttribute("index", (*arcIter)->properties->index);

        /* Pridani hrany do dokumentu */
        body.appendChild(arc);
    }

    return document;
}
