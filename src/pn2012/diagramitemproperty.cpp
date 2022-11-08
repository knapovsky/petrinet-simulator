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
 * Implementace tridy, ktera uchovava vlastnosti prvku simulacni sceny
 *
 */

#include "diagramitemproperty.h"


DiagramItemProperty::DiagramItemProperty(QString id_in,
                                         qreal x_in,
                                         qreal y_in,
                                         qreal width_in,
                                         qreal height_in,
                                         QColor color_in,
                                         int index_in,
                                         QString description_in)
{
    // Nastaveni promennych objektu
    id = id_in;
    x = x_in;
    y = y_in;
    width = width_in;
    height = height_in;
    color = color_in;
    index = index_in;
    description = description_in;
}

qreal DiagramItemProperty::getWidth(){

    return width;
}
qreal DiagramItemProperty::getHeight(){

    return height;
}
qreal DiagramItemProperty::getX(){

    return x;
}
qreal DiagramItemProperty::getY(){

    return y;
}
int DiagramItemProperty::getIndex(){

    return index;
}
QColor DiagramItemProperty::getColor(){

    return color;
}
QString DiagramItemProperty::getDescription(){

    return description;
}

QString DiagramItemProperty::getID(){

    return id;
}
