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
 * Hlavickovy soubor s definici custom Qt widgetu, ktery zobrazuje ohodnoceni mista
 * v seznamu a umoznuje jejich pridavani a editaci.
 *
 */

#ifndef LISTWIDGET_H
#define LISTWIDGET_H

//#include <QWidget>
//#include <QPushButton>
//#include <QListWidget>
#include "diagramplace.h"

class QWidget;
class QPushButton;
class QListWidget;

class ListWidget : public QWidget
{

  Q_OBJECT

  public:

    /**
      * Konstruktor
      * @param *parent Ukazatel na rodicovsky prvek
      * @return Ukazatel na nove vytvoreny objekt widgetu
      */
    ListWidget(QWidget *parent = 0);

    /**
      * Zobrazi ve widgetu vlastnosti daneho simulacniho mista
      * @param *diagramPlaceIn Misto, jehoz obsah se ma zobrazit
      */
    void setItem(DiagramPlace *diagramPlaceIn);

  private slots:

    /**
      * Prida do seznamu novou polozku
      */
    void addItem();

    /**
      * Odebere ze seznamu zvolenou polozku
      */
    void removeItem();

    /**
      * Odebere ze seznamu vsechny polozky
      */
    void clearItems();

  private:

    /**
      * List Widget
      */
    QListWidget *lw;

    /**
      * ADD Button
      */
    QPushButton *add;

    /**
      * Remove Button
      */
    QPushButton *remove;

    /**
      * Remove All Button
      */
    QPushButton *removeAll;

    /**
      * Ukazatel na zvolene misto
      */
    DiagramPlace *diagramPlace;
};

#endif
