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
 * Implementace tridy, ze ktere dedi vsechny prvky sceny.
 *
 */

#include "diagramitem.h"

DiagramItem::DiagramItem()
{
}

QRectF DiagramItem::boundingRect(){

    return QRectF(properties->x, properties->y, properties->width, properties->height);
}

bool DiagramItem::isInsideBoundingRect(QPointF point){

    //qDebug() << "Zjistuji bounding rect pro " << this->properties->id;
    QRectF rec = boundingRect();

    /* Pokud se jedna o prechod, vraci se ciste bounding rectangle */
    if(this->type == Transition){

        if(point.x() >= rec.x()
                && point.x() <= rec.x() + properties->width
                && point.y() >= rec.y()
                && point.y() <= rec.y() + properties->height
            ){
            return true;
        }
        else{
            return false;
        }
    }

    /* Jedna se o misto - musi se vypocitat, zda bod lezi uvnitr vepsane elipsy */
    else if(this->type == Place){

        qreal a = this->properties->width/2;
        qreal b = this->properties->height/2;
        qreal x0 = this->properties->x + a;
        // v qt je osa y invertovana
        qreal y0 = this->properties->y + b;

        qreal out = qPow(point.x() - x0, 2)/qPow(a, 2) + qPow(point.y() - y0, 2)/qPow(b, 2);

        //qDebug() << QString("a: %1, b: %2, x0: %3, y0: %4, x: %5, y: %6, out: %7").arg(a).arg(b).arg(x0).arg(y0).arg(point.x()).arg(point.y()).arg(out);
        if(out > 1) return false;
        else return true;
    }
    else{
        return false;
    }
}

void DiagramItem::prepareGeometryChangePublic()
{
    this->prepareGeometryChange();
}
