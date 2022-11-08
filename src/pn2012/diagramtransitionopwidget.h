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
 * Hlavickovy soubor s definici tridy, ktera predstavuje widget zobrazujici
 * operace, jez jsou pridruzeny k prechodu
 *
 */

#ifndef DIAGRAMTRANSITIONOPWIDGET_H
#define DIAGRAMTRANSITIONOPWIDGET_H

//#include <QWidget>
//#include <QPushButton>
//#include <QListWidget>
#include "diagramtransition.h"

class DiagramTransitionOpWidget : public QWidget
{

    Q_OBJECT

public:

    /**
      * Konstruktor
      * @param *parent Ukazatel na rodicovsky widget
      */
    DiagramTransitionOpWidget(QWidget *parent = 0);

    /**
      * Zobrazi ve widgetu operace prechodu
      * @param Prechod, ktery se ma zobrazit
      */
    void setItem(DiagramTransition *diagramTransitionIn);

private slots:

    /**
      * Vyvola nabidku na pridani operace k prechodu
      */
    void addItem();

    /**
      * Vyvola nabidku na zmenu vybraneho prvku
      */
    void changeItem();

    /**
      * Odstrani vybrany prvek
      */
    void removeItem();

    /**
      * Odstrani vsechny prvky
      */
    void clearItems();

private:

    /**
      * Ukazatel na ListWidget, ktery zobrazuje podminky prechodu
      */
    QListWidget *lw;

    /**
      * Ukazatel na ADD button
      */
    QPushButton *add;

    /**
      * Ukazatel na CHANGE button
      */
    QPushButton *change;

    /**
      * Ukazatel na REMOVE button
      */
    QPushButton *remove;

    /**
      * Ukazatel na REMOVEALL button
      */
    QPushButton *removeAll;

    /**
      * Ukazatel na prechod, jehoz podminky jsou zobrazeny
      */
    DiagramTransition *diagramTransition;
};

#endif // DIAGRAMTRANSITIONOPWIDGET_H
