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
 * Definice tridy, ktera je custom widgetem zobrazujicim nastaveni programu
 * a umoznujicim toto nastaveni zmenit
 *
 */

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

//#include <QtCore>
//#include <QtGui>
#include "settings.h"
#include "networklayer.h"

class SettingsWidget : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *settingsIn Ukazatel na globalni nastaveni programu
      * @param *dialogIn Ukazatel na dialogove okno obsahujici tento widget
      * @return Ukazatel na nove vytvoreny objekt
      */
    SettingsWidget(Settings *settingsIn, QDialog *dialogIn);

private:

    /**
      * Editor IP adresy
      */
    QLineEdit *ipE;

    /**
      * Editor Portu
      */
    QLineEdit *portE;

    /**
      * Editor uzivatelskeho jmena
      */
    QLineEdit *usernameE;

    /**
      * Editor hesla
      */
    QLineEdit *passwordE;

    /**
      * Editor cesty k souboru s nastavenim programu
      */
    QLineEdit *configFileE;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Nadrazene dialogove okno obsahujici tento widget
      */
    QDialog *dialog;

public slots:

    /**
      * Zobrazi nastaveni programu
      */
    void showSettings();

    /**
      * Ulozi zadane nastaveni programu
      */
    void setSettings();

    /**
      * Nacte soubor s nastavenim
      */
    void getConfigFile();
};

#endif // SETTINGSWIDGET_H
