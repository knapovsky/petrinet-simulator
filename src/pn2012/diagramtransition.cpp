#include "diagramtransition.h"
#include "diagramarc.h"
#include "simulationdata.h"

DiagramTransition::DiagramTransition(QString id_in,
                                     QList<QString> conditions_in,
                                     QList<QString> operations_in,
                                     qreal x_in,
                                     qreal y_in,
                                     qreal width_in,
                                     qreal height_in,
                                     QColor color_in,
                                     int index_in,
                                     QString description_in,
                                     Settings *&settingsIn,
                                     SimulationData *simDataIn)
{
    // Inicializace
    Pressed = false;
    conditions = conditions_in;
    operations = operations_in;
    settings = settingsIn;
    type = Transition;
    simData = simDataIn;
    boundX = x_in;
    boundY = y_in;
    delay = 0;

    // Objekt je pohyblivy
    setFlag(ItemIsMovable);

    // Vytvoreni objektu pro uschovani vlastnosti prechodu
    properties = new DiagramItemProperty(id_in,
                                         x_in,
                                         y_in,
                                         width_in,
                                         height_in,
                                         color_in,
                                         index_in,
                                         description_in);

}

QRectF DiagramTransition::boundingRect() const
{
    // Tady se bude menit poloha objektu a velikost
    return QRectF(boundX, boundY, properties->getWidth(), properties->getHeight());
}

void DiagramTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();

    // Nastaveni brushe
    QBrush brush(Qt::blue);

    // Nastaveni barvy stlaceneho objektu
    if(Pressed){
        brush.setColor(Qt::red);
    }
    // Nestlaceny
    else{
        brush.setColor(properties->getColor());
    }

    // Nastaveni pera podle toho, zda je objekt zvolen
    if(settings->selectedItem == this){
        painter->setPen(settings->selectedPen);
    }
    else{
        painter->setPen(settings->notSelectedPen);
    }

    // Vytvoreni retezce s podminkami
    // Slouzi pro zobrazeni podminek uvnitr graficke
    // reprezentace prechodu
    QString conditionString;
    foreach(QString condition, conditions){
        conditionString.append(condition);
        conditionString.append("\n");
    }

    // Vytvoreni retezce s operacemi
    // Slouzi pro zobrazeni podminek uvnitr graficke
    // reprezentace prechodu
    QString operationString;
    foreach(QString operation, operations){
        operationString.append(operation);
        if(operation != operations.last()){
            operationString.append("\n");
        }
    }

    // Souhrnny zobrazovany retezec
    QString valuesString;
    valuesString.append(conditionString);
    valuesString.append(operationString);

    // Nastaveni pisma a overeni toho,
    // zda se retezec s podminkami a operacemi
    // vleze do prechodu
    // Pokud ne, vypise se "..." a pokud se
    // ani to nevejde, pak se nevypise nic
    QFont f("Helvetica", 12);
    painter->setFont(f);
    QFontMetrics fm = painter->fontMetrics();
    if(properties->getWidth() < fm.width(valuesString)){
        if(fm.width("...") < properties->getWidth()){
            valuesString = "...";
        }
        else{
            valuesString = "";
        }
    }

    // Nastaveni vrstvy, do ktere se prechod vykresluje
    this->setZValue(properties->index);

    // Vyplneni prechodu barvou
    painter->fillRect(rec, brush);

    // Vykresleni prechodu
    painter->drawRect(rec);

    // Vykresleni textu do prechodu
    painter->drawText(rec, Qt::AlignCenter, valuesString);
}

QString DiagramTransition::getID()
{
    return properties->getID();
}

qint32 DiagramTransition::getDelay()
{
    return delay;
}

void DiagramTransition::setDelay(qint32 delayIn)
{
    delay = delayIn;
}

void DiagramTransition::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    // Vybere prechod
    Pressed = true;
    this->type = Transition;
    settings->setSelectedItem(this);

    // Aktualizace sceny
    update();

    // Delegace eventu
    QGraphicsItem::mousePressEvent(event);
}

void	DiagramTransition::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ){

    // Priprava na aktualizaci sceny
    prepareGeometryChange();

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();

    // Aktualizace polohy prechodu
    this->properties->x = rec.x() + this->pos().x();
    this->properties->y = rec.y() + this->pos().y();

    /* Update hran spojenych s prechodem */
    QList<DiagramArc*>::iterator iter;
    for(iter = simData->diagramArcs.begin(); iter != simData->diagramArcs.end(); iter++){

        // Aktualizace sceny
        (*iter)->prepareGeometryChangePublic();
        (*iter)->update();
    }

    QGraphicsItem::mouseMoveEvent(event);


}

void DiagramTransition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    // Priprava na aktualizaci sceny
    prepareGeometryChange();

    // Prechod neni vybran
    Pressed = false;

    // Ohranicujici obdelnik
    QRectF rec = boundingRect();

    // Aktualizace polohy prechodu
    this->properties->x = rec.x() + this->pos().x();
    this->properties->y = rec.y() + this->pos().y();

    /* Update hran spojenych s prechodem */
    QList<DiagramArc*>::iterator iter;
    for(iter = simData->diagramArcs.begin(); iter != simData->diagramArcs.end(); iter++){

        // Aktualizace sceny
        (*iter)->prepareGeometryChangePublic();
        (*iter)->update();
    }

    // Delegace eventu
    QGraphicsItem::mouseReleaseEvent(event);
}
