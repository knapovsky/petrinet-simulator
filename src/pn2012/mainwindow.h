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
 * Hlavickovy soubor s definici tridy, ktera reprezentuje hlavni okno programu.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtCore>
//#include <QtGui>
#include "simlist.h"
#include "settings.h"
#include "networklayer.h"
#include "settingswidget.h"
#include "networkbrowser.h"
#include "simulationpropertieswidget.h"

class DiagramScene;
class Simulation;


class MainWindow : public QMainWindow
{

    Q_OBJECT

    
public:

    /**
      * Konstruktor
      * @return Ukazatel na vytvoreny objekt
      */
    MainWindow(QWidget *parent = 0);

    /**
      * Ziska ukazatel na soucasne aktivni simulaci
      * @return Ukazatel na aktivni simulaci
      */
    Simulation* getCurrentSimulation();

private slots:

    /** Menu Section **/

    /* File */

    /**
      * Vytvori novou simulaci
      */
    void newFile();

    /**
      * Otevre simulacni soubor
      */
    void open();

    /**
      * Ulozi simulacni soubor na disk
      */
    bool save();

    /**
      * Ulozi simulacni soubor na disk - ulozit jako
      */
    bool saveAs();

    /**
      * Pripoji na server
      */
    bool connectToServer();

    /**
      * Odpoji ze serveru
      */
    bool disconnectFromServer();

    /**
      * Ulozi simulacni soubor na server
      */
    bool saveToServer();

    /**
      * Otevre okno s prochazenim siti na serveru
      */
    void browse();

    /**
      * Ukonci program
      */
    void exit();

    /* Edit */

    /**
      * Vyjme aktualne zvolenou polozku
      */
    void cut();

    /**
      * Zkopiruje aktualne zvolenou polozku
      */
    void copy();

    /**
      * Vlozi vyjmutou, nebo zkopirovanou polozku
      */
    void paste();

    /* Simulation */

    /**
      * Spusti simulaci na serveru
      */
    void runSimulation();

    /**
      * Provede krok simulace
      */
    void nextStep();
    //void previousStep(); // smazat

    /**
      * Restartuje simulaci
      */
    void resetSimulation();

    /**
      * Otevre okno s editaci vlastnosti simulace
      */
    void editSimulationProperties();

    /* Tools */

    /**
      * Otevre okno s nastavenim programu
      */
    void settings(); // ip, port, barvy, fonty

    /**
      * Nacte puvodni nastaveni programu
      */
    void loadDefaultSettings();

    /**
      * Ulozi nastaveni programu do config souboru
      */
    void saveSettings();

    /* Help */

    /**
      * Otevre napovedu
      */
    void launchHelp(); // qt help

    /**
      * Otevre about dialog
      */
    void about();

    /* Modes */

    /**
      * Prepne na mod vyberu - kurzor
      */
    void mouseMode();

    /**
      * Prepne na mod vkladani mist
      */
    void placeMode();

    /**
      * Prepne na moc vkladani prechodu
      */
    void transitionMode();

    /**
      * Prepne na mod vkladani hran
      */
    void arcMode();

    /**
      * Prepina mezi mody
      */
    void switchMode();

    /* delete */

    /**
      * Vysle delete event
      */
    void sendDelete();

    /**
      * Aktualizuje nazev aktualni simulace
      */
    void updateSimName();

    /**
      * Prepina to, zda zobrazovat vlastnosti prvku sceny, ci nikoliv
      */
    void setDisplayProperties();

    /** End of Menu Section **/

private:

    /**
      * Vytvori Qt akce
      */
    void createActions();

    /**
      * Vytvori menu programu
      */
    void createMenus();

    /**
      * Vytvori toolbary
      */
    void createToolbars();

    /**
      * Vytvori vnitrni widgety
      */
    void createCentralWidget();

    /**
      * Vytvori status bar
      */
    void createStatusBar();

    /**
      * Dialog s potvrzenim zmeny
      */
    bool okToContinue();

    /** MENU */

    /**
      * Hlavni menu
      */
    QMenu *fileMenu;

    /**
      * Editacni menu
      */
    QMenu *editMenu;

    /**
      * Simulacni menu
      */
    QMenu *simulationMenu;

    /**
      * Menu s nastroji
      */
    QMenu *toolsMenu;

    /**
      * Menu s napovedou
      */
    QMenu *helpMenu;

    /** Toolbars **/
    QToolBar *fileToolbar;
    QToolBar *editToolbar;
    QToolBar *simulationToolbar;
    QToolBar *modeToolbar;

    /** Menu Section **/

    /** File **/
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *connectAction;
    QAction *disConnectAction;
    QAction *saveToServerAction;
    QAction *browseAction;
    QAction *exitAction;

    /** Edit **/
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

    /** Simulation **/
    QAction *runSimulationAction;
    QAction *nextStepAction;
    QAction *previousStepAction;
    QAction *resetSimulationAction;
    QAction *editSimulationPropertiesAction;

    /** Settings **/
    QAction *settingsAction;
    QAction *loadDefaultSettingsAction;
    QAction *saveSettingsAction;

    /** Help **/
    QAction *launchHelpAction;
    QAction *aboutAction;
    QAction *qtAction;

    /** Modes **/
    QAction *mouseModeAction;
    QAction *placeModeAction;
    QAction *transitionModeAction;
    QAction *arcModeAction;

    QAction *removeAction;

    QAction *displayPropertiesAction;

    /** End of Menu Section **/

    /**
      * Central Widget
      */
    SimList *simList;

    /**
      * Aktualni mod mysi
      */
    Settings *modeObject;
};

#endif // MAINWINDOW_H
