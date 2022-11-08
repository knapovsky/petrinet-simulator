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
 * Hlavickovy soubor s definici tridy, ktera slouzi k uchovani spolecnych vlastnosti
 * prvku simulacni sceny.
 *
 */

#ifndef DIAGRAMITEMPROPERTY_H
#define DIAGRAMITEMPROPERTY_H

#include <QColor>
#include <QString>

class DiagramItemProperty
{
public:    

    /**
      * Konstruktor objektu
      * @param id_in ID prvku
      * @param x_in Pozice prvku na ose X
      * @param y_in Pozice prvku na ose Y
      * @param width_in Sirka prvku
      * @param height_in Vyska prvku
      * @param color_in Barva prvku
      * @param index_in Vrstva, ve ktere se prvek zobrazuje
      * @param description_in Textovy popis prvku
      * @return Ukazatel na vytvoreny objekt
      */
    DiagramItemProperty(QString id_in,
                        qreal x_in,
                        qreal y_in,
                        qreal width_in,
                        qreal height_in,
                        QColor color_in,
                        int index_in,
                        QString description_in);

    /**
      * Ziska sirku prvku
      * @return Sirka prvku
      */
    qreal getWidth();

    /**
      * Ziska vysku prvku
      * @return Vyska prvku
      */
    qreal getHeight();

    /**
      * Ziska pozici na ose X
      * @return Pozice na ose X
      */
    qreal getX();

    /**
      * Ziska pozici na ose Y
      * @return Pozice na ose Y
      */
    qreal getY();

    /**
      * Ziska vrstrvu, ve ktere se prvek nachazi
      * @return Vrstva, ve ktere se prvek nachazi
      */
    int getIndex();

    /**
      * Ziska barvu prvku
      * @return Barva prvku
      */
    QColor getColor();

    /**
      * Ziska textovy popis prvku
      * @return Textovy popis prvku
      */
    QString getDescription();

    /**
      * Ziska ID prvku
      * @return ID prvku
      */
    QString getID();

    /**
      * Sirka prvku
      */
    qreal width;

    /**
      * Vyska prvku
      */
    qreal height;

    /**
      * Barva prvku
      */
    QColor color;

    /**
      * Pozice na ose X
      */
    qreal x;

    /**
      * Pozice na ose Y
      */
    qreal y;

    /**
      * Vrstva, ve ktere se prvek nachazi
      */
    int index;

    /**
      * Textovy popis prvku
      */
    QString description;

    /**
      * ID prvku
      */
    QString id;

};

#endif // DIAGRAMITEMPROPERTY_H
