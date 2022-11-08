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
 * Definice tridy, ktera reprezentuje custom Qt widget, pres ktery se nastavuji
 * vlastnosti simulace - Autor, Popis, ...
 *
 */

#ifndef SIMULATIONPROPERTIESWIDGET_H
#define SIMULATIONPROPERTIESWIDGET_H
//#include <QtCore>
//#include <QtGui>
//#include <QDebug>
#include "settings.h"
#include "simulationdata.h"

class SimulationPropertiesWidget : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *simDataIn Ukazatel na data simulace
      * @param *setttingsIn Ukazatel na globalni nastaveni programu
      * @param *dialogIn Ukazatel na dialogove okno, v nemz je tento widget obsazen
      * @return Ukazatel na nove vytvoreny objekt
      */
    SimulationPropertiesWidget(SimulationData *simDataIn,
                               Settings *settingsIn,
                               QDialog *dialogIn
                               );

public slots:

    /**
      * Zobrazi vlastnosti simulace do prvku widgetu
      */
    void showProperties();

    /**
      * Nastavi zadane vlastnosti simulace v simulacnich datech
      */
    void setProperties();

private:

    /**
      * Ukazatel na simulacni data
      */
    SimulationData *simData;

    /**
      * Ukazatel na globalni nastaveni programu
      */
    Settings *settings;

    /**
      * Ukazatel na dialogove okno, ktere obsahuje tento widget
      */
    QDialog *dialog;

    /**
      * Name Line Edit
      */
    QLineEdit *nameE;

    /**
      * Author Line Edit
      */
    QLineEdit *authorE;

    /**
      * Version Line Edit
      */
    QLineEdit *versionE;

    /**
      * Description Line Edit
      */
    QTextEdit *descriptionE;

    /**
      * Date Line Edit
      */
    QLineEdit *dateE;

};

#endif // SIMULATIONPROPERTIESWIDGET_H
