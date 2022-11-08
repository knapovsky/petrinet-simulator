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
 * Implementace tridy predstavujici hlavni okno programu.
 *
 */

#include <QtGui>
#include <QLabel>

#include "mainwindow.h"
#include <QDesktopServices>
#include <QCoreApplication>

const int InsertTextButton = 10;

MainWindow::MainWindow(QWidget *parent){


    /* Vytvoreni prvku okna */
    createActions();
    createMenus();
    createToolbars();

    /* Vytvoreni objektu globalniho nastaveni */
    modeObject = new Settings(ModePointer);
    if(modeObject == NULL) return;
    modeObject->setMainWindowPointer(this);

    /* Seznam Simulaci */
    simList = new SimList(modeObject);

    /* Vytvoreni sitove vrstvy */
    modeObject->net = new NetworkLayer(modeObject, simList);

    /* Vytvoreni status baru */
    createStatusBar();

    /* Vyber prvku sceny*/
    connect(modeObject, SIGNAL(itemSelected(DiagramItem*)), simList, SLOT(itemSelected(DiagramItem*)));

    /* Ikona programu */
    setWindowIcon(QIcon(":/images/icon.png"));

    /* Nastaveni hlavniho widgetu */
    setCentralWidget(simList);
}

Simulation* MainWindow::getCurrentSimulation()
{
    // Vraci aktualne zvolenou simulaci
    return simList->currentSimulation;
}

void MainWindow::createActions(){

    /* File */
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create new High-Level Petri Net"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open High-Level Petri Net"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save High-Level Petri Net"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("&Save As"), this);
    saveAsAction->setIcon(QIcon(":images/saveas.png"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save High-Level Petri Net As"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    connectAction = new QAction(tr("&Connect"), this);
    connectAction->setIcon(QIcon(":images/connect.png"));
    //connectAction->setShortcut(QKeySequence::Connect);
    connectAction->setStatusTip(tr("Connect to Server"));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectToServer()));

    disConnectAction = new QAction(tr("&Disconnect"), this);
    disConnectAction->setIcon(QIcon(":images/disconnect.png"));
    //disConnectAction->setShortcut(QKeySequence::disConnect);
    disConnectAction->setStatusTip(tr("Disconnect from Server"));
    connect(disConnectAction, SIGNAL(triggered()), this, SLOT(disconnectFromServer()));

    saveToServerAction = new QAction(tr("Save to server"), this);
    saveToServerAction->setIcon(QIcon(":images/savetoserver.png"));
    //saveToServerAction->setShortcut(QKeySequence::disConnect);
    saveToServerAction->setStatusTip(tr("Save to Server"));
    connect(saveToServerAction, SIGNAL(triggered()), this, SLOT(saveToServer()));

    browseAction = new QAction(tr("&Browse"), this);
    browseAction->setIcon(QIcon(":images/browse.png"));
    //browseAction->setShortcut(QKeySequence::browse);
    browseAction->setStatusTip(tr("Browse Petri Nets on Server"));
    connect(browseAction, SIGNAL(triggered()), this, SLOT(browse()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setIcon(QIcon(":images/quit.png"));
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Editor"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    /* Edit */
    cutAction = new QAction(tr("Cut"), this);
    cutAction->setIcon(QIcon(":images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut Selected Item"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setIcon(QIcon(":images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy Selected Item"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setIcon(QIcon(":images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste Item"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    /* Simulation */
    runSimulationAction = new QAction(tr("Run Simulation"), this);
    runSimulationAction->setIcon(QIcon(":images/runsim.png"));
    //runSimulationAction->setShortcut(QKeySequence::Copy);
    runSimulationAction->setStatusTip(tr("Run Simulation on Server"));
    connect(runSimulationAction, SIGNAL(triggered()), this, SLOT(runSimulation()));

    nextStepAction = new QAction(tr("Next Step"), this);
    nextStepAction->setIcon(QIcon(":images/nextstep.png"));
    //nextStepAction->setShortcut(QKeySequence::Copy);
    nextStepAction->setStatusTip(tr("Next Step in Simulation on Server"));
    connect(nextStepAction, SIGNAL(triggered()), this, SLOT(nextStep()));

    previousStepAction = new QAction(tr("Previous Step"), this);
    previousStepAction->setIcon(QIcon(":images/previousstep.png"));
    //previousStepAction->setShortcut(QKeySequence::Copy);
    previousStepAction->setStatusTip(tr("Previous Step in Simulation on Server"));
    //connect(previousStepAction, SIGNAL(triggered()), this, SLOT(previousStep()));

    resetSimulationAction = new QAction(tr("Reset Simulation"), this);
    resetSimulationAction->setIcon(QIcon(":images/ressim.png"));
    //resetSimulationAction->setShortcut(QKeySequence::Copy);

    resetSimulationAction->setStatusTip(tr("Reset Simulation on Server"));
    connect(resetSimulationAction, SIGNAL(triggered()), this, SLOT(resetSimulation()));

    editSimulationPropertiesAction = new QAction(tr("Edit Simulation Properties"), this);
    editSimulationPropertiesAction->setIcon(QIcon(":images/editsimprops.png"));
    editSimulationPropertiesAction->setStatusTip(tr("Run Simulation on Server"));
    connect(editSimulationPropertiesAction, SIGNAL(triggered()), this, SLOT(editSimulationProperties()));

    /* Tools */
    settingsAction = new QAction(tr("Settings"), this);
    settingsAction->setIcon(QIcon(":images/settings.png"));
    //settingsAction->setShortcut(QKeySequence::Options);
    settingsAction->setStatusTip(tr("Set Server IP and Simulation Step Size"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(settings()));

    loadDefaultSettingsAction = new QAction(tr("Load Default Settings"), this);
    //loadDefaultSettingsAction->setIcon(QIcon(":images/loaddefault.png"));
    //loadDefaultSettingsAction->setShortcut(QKeySequence::Options);
    loadDefaultSettingsAction->setStatusTip(tr("Loads default settings of program"));
    connect(loadDefaultSettingsAction, SIGNAL(triggered()), this, SLOT(loadDefaultSettings()));

    saveSettingsAction = new QAction(tr("Save Settings"), this);
    //saveSettingsAction->setIcon(QIcon(":images/savesettings.png"));
    //saveSettingsAction->setShortcut(QKeySequence::Options);
    saveSettingsAction->setStatusTip(tr("Save settings to config file"));
    connect(saveSettingsAction, SIGNAL(triggered()), this, SLOT(saveSettings()));

    /* Help */
    launchHelpAction = new QAction(tr("Launch Help"), this);
    launchHelpAction->setIcon(QIcon(":images/help.png"));
    launchHelpAction->setShortcut(QKeySequence::HelpContents);
    launchHelpAction->setStatusTip(tr("Launch Help"));
    connect(launchHelpAction, SIGNAL(triggered()), this, SLOT(launchHelp()));

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setIcon(QIcon(":images/about.png"));
    //aboutAction->setShortcut(QKeySequence::Ab);
    aboutAction->setStatusTip(tr("Information about this Application"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    /* Modes */
    mouseModeAction = new QAction(tr("Mouse"), this);
    mouseModeAction->setIcon(QIcon(":images/pointer.png"));
    mouseModeAction->setStatusTip(tr("Moving objects"));
    mouseModeAction->setCheckable(true);
    mouseModeAction->setChecked(true);
    connect(mouseModeAction, SIGNAL(triggered()), this, SLOT(mouseMode()));
    //connect(mouseModeAction, SIGNAL(triggered()), this, SLOT(switchMode()));

    placeModeAction = new QAction(tr("Place"), this);
    placeModeAction->setIcon(QIcon(":images/place.png"));
    placeModeAction->setStatusTip(tr("Insert place"));
    placeModeAction->setCheckable(true);
    connect(placeModeAction, SIGNAL(triggered()), this, SLOT(placeMode()));
    //connect(placeModeAction, SIGNAL(triggered()), this, SLOT(switchMode()));

    transitionModeAction = new QAction(tr("Transition"), this);
    transitionModeAction->setIcon(QIcon(":images/transition.png"));
    transitionModeAction->setStatusTip(tr("Insert transition"));
    transitionModeAction->setCheckable(true);
    connect(transitionModeAction, SIGNAL(triggered()), this, SLOT(transitionMode()));
    //connect(transitionModeAction, SIGNAL(triggered()), this, SLOT(switchMode()));

    arcModeAction = new QAction(tr("Arc"), this);
    arcModeAction->setIcon(QIcon(":images/arc.png"));
    arcModeAction->setStatusTip(tr("Insert arc"));
    arcModeAction->setCheckable(true);
    connect(arcModeAction, SIGNAL(triggered()), this, SLOT(arcMode()));
    //connect(arcModeAction, SIGNAL(triggered()), this, SLOT(switchMode()));

    displayPropertiesAction = new QAction(tr("Display Properties"), this);
    displayPropertiesAction->setIcon(QIcon(":images/display.png"));
    displayPropertiesAction->setStatusTip(tr("Display item properties"));
    displayPropertiesAction->setCheckable(true);
    connect(displayPropertiesAction, SIGNAL(triggered()), this, SLOT(setDisplayProperties()));
    displayPropertiesAction->setChecked(true);

    /* remove action */
    removeAction = new QAction(tr("Remove"), this);
    removeAction->setIcon(QIcon(":images/remove.png"));
    removeAction->setShortcut(Qt::Key_Delete);
    removeAction->setStatusTip(tr("Remove selected item"));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(sendDelete()));

}

void MainWindow::createMenus(){

    /* File */
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(connectAction);
    fileMenu->addAction(disConnectAction);
    fileMenu->addAction(saveToServerAction);
    fileMenu->addAction(browseAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    /* Edit */
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(removeAction);

    /* Simulation */
    simulationMenu = menuBar()->addMenu(tr("&Simulation"));
    simulationMenu->addAction(editSimulationPropertiesAction);
    simulationMenu->addSeparator();
    simulationMenu->addAction(runSimulationAction);
    simulationMenu->addAction(nextStepAction);
    simulationMenu->addAction(resetSimulationAction);

    /* Tools */
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(settingsAction);
    toolsMenu->addAction(loadDefaultSettingsAction);
    toolsMenu->addAction(saveSettingsAction);

    /* Help */
    helpMenu = menuBar()->addMenu(tr("&About"));
    //helpMenu->addAction(launchHelpAction);
    helpMenu->addAction(aboutAction);
    //helpMenu->addAction(qtAction);

}

void MainWindow::createToolbars(){

    fileToolbar = addToolBar(tr("&File"));
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(newAction);
    fileToolbar->addAction(saveAction);

    editToolbar = addToolBar(tr("&Edit"));
    editToolbar->addAction(cutAction);
    editToolbar->addAction(copyAction);
    editToolbar->addAction(pasteAction);
    editToolbar->addAction(removeAction);

    simulationToolbar = addToolBar(tr("&Simulation"));
    simulationToolbar->addAction(editSimulationPropertiesAction);
    simulationToolbar->addAction(runSimulationAction);
    simulationToolbar->addAction(nextStepAction);
    simulationToolbar->addAction(resetSimulationAction);

    modeToolbar = addToolBar(tr("&Mode"));
    modeToolbar->addAction(mouseModeAction);
    modeToolbar->addAction(placeModeAction);
    modeToolbar->addAction(transitionModeAction);
    modeToolbar->addAction(arcModeAction);
    modeToolbar->addAction(displayPropertiesAction);

}

void MainWindow::createCentralWidget(){
    ;
}

void MainWindow::createStatusBar(){
    //updateStatusBar();
    statusBar()->showMessage("Ready");
}


bool MainWindow::okToContinue()
{
    //if (isWindowModified()) {
    if(true){
        int r = QMessageBox::warning(this, tr("Petri Net"),
        tr("The net has been modified.\n"
        "Do you want to save your changes?"),
        QMessageBox::Yes | QMessageBox::No
        | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) {
        return save();
    } else if (r == QMessageBox::Cancel) {
        return false;
      }
    }
    return true;
}

/* File */
void MainWindow::newFile(){

    qDebug() << "Creating new file";

    // Prida simulaci do seznamu simulaci
    simList->addSimulation("", modeObject, "");
    connect(simList, SIGNAL(currentChanged(int)), simList, SLOT(changeCurrentSimulation(int)));
    return;
}

void MainWindow::open(){

    // Vyzaduje potvrzeni od uzivatele
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                            tr("Open High-Level Petri Net"), ".",
                            tr("High-Level Petri Net Files (*.xml)"));
        if (fileName.isEmpty()){ return;}

        // Vytvori simulaci a nacte jeji data ze souboru
        simList->addSimulation(fileName, modeObject, "");
        connect(simList, SIGNAL(currentChanged(int)), simList, SLOT(changeCurrentSimulation(int)));
    }
}

bool MainWindow::save(){

    qDebug() << "Saving: " << ((Simulation*)(simList->currentWidget()))->getFilename();
    ((Simulation*)(simList->currentWidget()))->saveSimulationLocally();
    modeObject->saveSettingsToXML();
    return true;
}

bool MainWindow::saveAs(){

    qDebug() << "Saving As: " << ((Simulation*)(simList->currentWidget()))->getFilename();
    ((Simulation*)(simList->currentWidget()))->saveAsSimulationLocally();
    return true;
}

bool MainWindow::connectToServer(){

    qDebug() << "Connecting to server";
    /* Vytvoreni network layer objektu, ulozeni ukazatele na nej do settings, pripojeni k serveru */
    modeObject->net->connectToServer(modeObject->ip, modeObject->port);
    //modeObject->net->sendLogin();

    return true;
}

bool MainWindow::disconnectFromServer()
{
    qDebug() << "Disconnecting from server";
    modeObject->net->disconnectFromServer();
    simList->disconnectFromServer();

    modeObject->showInfo("You have been disconnected from server");

    return true;
}

bool MainWindow::saveToServer()
{
    qDebug() << "Saving to server";
    modeObject->net->sendSaveNet();

    return true;
}

void MainWindow::browse(){

    qDebug() << "Browsing on server";
    if(modeObject->net->getLoginStatus()){
        modeObject->net->sendGetList();
    }
    else{
        this->statusBar()->showMessage("You must login first in order to browse nets on server");
        modeObject->showAlert("You must login first in order to browse nets on server");
    }
    return;
}

void MainWindow::exit(){

    //modeObject->saveSettingsToXML();

    this->close();
}

/* Edit */
void MainWindow::cut(){

    qDebug() << "Cutting selected item";
    simList->currentSimulation->getSimData()->cutSelectedItem();
    return;
}

void MainWindow::copy(){

    qDebug() << "Copying selected item";
    simList->currentSimulation->getSimData()->copySelectedItem();
    return;
}

void MainWindow::paste(){

    qDebug() << "Pasting selected item";
    simList->currentSimulation->getSimData()->pasteItem();
    return;
}


/* Simulation */
void MainWindow::runSimulation(){

    qDebug() << "Running simulation of: " << ((Simulation*)(simList->currentWidget()))->getFilename();
    ((Simulation*)(simList->currentWidget()))->runSimulation();
}

void MainWindow::nextStep(){

    qDebug() << "Next step in simulation: " << ((Simulation*)(simList->currentWidget()))->getFilename();
    ((Simulation*)(simList->currentWidget()))->nextStepSimulation();
    return;
}

void MainWindow::resetSimulation(){

    qDebug() << "Restarting simulation: " << ((Simulation*)(simList->currentWidget()))->getFilename();
    ((Simulation*)(simList->currentWidget()))->restartSimulation();
    return;
}

void MainWindow::editSimulationProperties()
{
    qDebug() << "Opening edit simulation properties dialog";
    QDialog *dialogWindow = new QDialog(this);

    // Widget na upravu vlastnosti site
    SimulationPropertiesWidget *simPropWidget = new SimulationPropertiesWidget(simList->currentSimulation->getSimData(), modeObject, dialogWindow);
    QVBoxLayout *dialogLayout = new QVBoxLayout;

    // Buttony a navazani jejich signalu
    QPushButton *ok = new QPushButton("OK", this);
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(ok, SIGNAL(clicked()), simPropWidget, SLOT(setProperties()));
    connect(ok, SIGNAL(clicked()), this, SLOT(updateSimName()));
    connect(cancel, SIGNAL(clicked()), dialogWindow, SLOT(reject()));

    // Layouty
    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(ok);
    buttons->addWidget(cancel);

    dialogLayout->addWidget(simPropWidget);
    dialogLayout->addLayout(buttons);

    dialogWindow->setLayout(dialogLayout);

    dialogWindow->show();
}

void MainWindow::updateSimName(){

    qDebug() << "updating";
    simList->setTabText(simList->currentIndex(), simList->currentSimulation->getSimData()->name);
}


/* Tools */
void MainWindow::settings(){

    qDebug() << "Opening settings dialog";
    QDialog *dialogWindow = new QDialog(this);

    // Widget s nastaveni programu
    SettingsWidget *settingsWidget = new SettingsWidget(modeObject, dialogWindow);
    QVBoxLayout *dialogLayout = new QVBoxLayout;

    QPushButton *ok = new QPushButton("OK", this);
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(ok, SIGNAL(clicked()), settingsWidget, SLOT(setSettings()));
    connect(cancel, SIGNAL(clicked()), dialogWindow, SLOT(reject()));

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(ok);
    buttons->addWidget(cancel);

    dialogLayout->addWidget(settingsWidget);
    dialogLayout->addLayout(buttons);

    dialogWindow->setLayout(dialogLayout);

    dialogWindow->show();
}


/* Help */
void MainWindow::launchHelp(){

    qDebug() << "Opening help";

  //  QWebView *view = new QWebView(parent);
   // view->load(QUrl("http://www.trolltech.com/"));
    //view->show();
    //QString currentDirPath =  QDir::currentPath();
    //qDebug() << "PATH: " << currentDirPath;
    //QDesktopServices::openUrl(QUrl("file://../../doc/html/index.html", QUrl::TolerantMode));


    return;
}

void MainWindow::about(){

    qDebug() << "Opening about dialog";
    QMessageBox::about(this, tr("About High-Level Petri Net Simulator"),
                       tr("This is school assignment for ICP@FIT VUT Brno\nAuthors\n Client: Martin Knapovsky - xknapo02@stud.fit.vutbr.cz \n Server: Martin Cincala - xcinca00@stud.fit.vutbr.cz \nFor help use make doxygen which generates help into doc/html/index.html."));
}

/* Modes */
void MainWindow::mouseMode(){

    modeObject->setCurrentMode(ModePointer);
    switchMode();
}
void MainWindow::placeMode(){

    modeObject->setCurrentMode(ModePlace);
    switchMode();
}
void MainWindow::transitionMode(){

    modeObject->setCurrentMode(ModeTransition);
    switchMode();
}
void MainWindow::arcMode(){

    modeObject->setCurrentMode(ModeArc);
    switchMode();
}

void MainWindow::switchMode()
{
    //qDebug() << mode;
    mouseModeAction->setChecked(modeObject->getCurrentMode() == ModePointer);
    placeModeAction->setChecked(modeObject->getCurrentMode() == ModePlace);
    transitionModeAction->setChecked(modeObject->getCurrentMode() == ModeTransition);
    arcModeAction->setChecked(modeObject->getCurrentMode() == ModeArc);
}

void MainWindow::sendDelete()
{
    QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
    QCoreApplication::postEvent(simList->currentSimulation->getScene(), event);

}

void MainWindow::loadDefaultSettings(){

    qDebug() << "Loading default settings";
    modeObject->loadDefaultSettings();
}

void MainWindow::saveSettings(){

    qDebug() << "Saving settings to config.xml";
    modeObject->saveSettingsToXML();
}

void MainWindow::setDisplayProperties(){

    if(modeObject->displayProperties){
        modeObject->displayProperties = false;
        displayPropertiesAction->setChecked(false);
        if(modeObject->selectedItem){
            simList->currentSimulation->resizeProps();
        }
    }
    else{
        modeObject->displayProperties = true;
        displayPropertiesAction->setChecked(true);
        if(modeObject->selectedItem){
            simList->currentSimulation->itemSelected(modeObject->selectedItem);
        }
    }
}




