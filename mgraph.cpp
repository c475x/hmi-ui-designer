#include <QPainter>
#include <QStaticText>
#include "mgraph.h"

// ----------------------------------------- ICON ---------------------------------------

MIconSet::MIconSet(QPointF origin, QSizeF size)
{
    type = "icon";
    props = new MItemProperty(GuiIconSet);
    //current = -1;
    //this->position = QRectF(origin, size);
    props->setProperty(0, origin.x());
    props->setProperty(1, origin.y());
    props->setProperty(2, size.width());
    props->setProperty(3, size.height());
}

QRectF MIconSet::boundingRect() const
{
    //return position;
    return QRectF(
        QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
        PC_SCALE * props->getProperty(1)->data.toInt()),
        QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
        PC_SCALE * props->getProperty(3)->data.toInt()));
}

void MIconSet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // Рисуем синий прямоугольник
    QPen pen(Qt::DashLine);
    QBrush brush(selected ? Qt::green : Qt::white);
    pen.setColor(selected ? Qt::red : Qt::blue);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());
    int16_t current = props->getProperty(5)->data.toInt();
    if((current >= 0) && (current < set.size()))
    {
        painter->setPen(Qt::SolidLine);
        painter->setPen(Qt::black);
        painter->drawImage(boundingRect(), set.at(current));
    }
    else
    {
        // Рисуем синий крест
        painter->drawLine(boundingRect().topLeft(), boundingRect().bottomRight());
        painter->drawLine(boundingRect().topRight(), boundingRect().bottomLeft());
    }
}

void MIconSet::addIcon(QImage icon)
{
    set.append(icon);
}

/**
 * @brief MIconSet::clearIcons - функция очищает список изображений иконки
 */
void MIconSet::clearIcons()
{
    set.clear();
}

void MIconSet::setCurrent(int16_t pos)
{
    if(pos < set.size())
    {
        props->setProperty(5, pos);
        //current = pos;
    }
}

// -------------------------------------------- LABEL ------------------------------------------------
MLabel::MLabel(QRectF position, QString str, uint8_t font)
{
    type = "labl";
    props = new MItemProperty(GuiLabel);
    props->setProperty(0, position.topLeft().x());
    props->setProperty(1, position.topLeft().y());
    props->setProperty(2, position.width());
    props->setProperty(3, position.height());
    props->setProperty(4, str);
    props->setProperty(5, font);
}

QRectF MLabel::boundingRect() const
{
    //return position;
    return QRectF(
        QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
        PC_SCALE * props->getProperty(1)->data.toInt()),
        QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
        PC_SCALE * props->getProperty(3)->data.toInt()));
}

void MLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // Рисуем синий прямоугольник
    QPen pen(Qt::DashLine);
    QBrush brush(selected ? Qt::green : Qt::white);
    //pen.setColor(selected ? Qt::red : Qt::blue);
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    switch(props->getProperty(5)->data.toInt())
    {
        case 0:
            painter->setFont(QFont("Courier New", 30));
        break;
    }

    painter->setPen(Qt::black);
    //painter->drawText(position.topLeft(), str);
    painter->drawStaticText(boundingRect().topLeft(), QStaticText(props->getProperty(4)->data.toString()));
}

// ---------------------------------------- PROGRESS ---------------------------------------------
//MProgress::MProgress()
//{
//    min = 0;
//    max = 100;
//    val = 0;
//    position = QRectF(0, 0, 400, 32);
//    type = "prgs";
//}

MProgress::MProgress(QRectF position)
{
    //min = 0;
    //max = 100;
    //val = 0;
    //this->position = position;
    type = "prgs";
    props = new MItemProperty(GuiProgress);
    props->setProperty(0, position.topLeft().x());
    props->setProperty(1, position.topLeft().y());
    props->setProperty(2, position.width());
    props->setProperty(3, position.height());
    props->setProperty(4, 0);   // value
    props->setProperty(5, "");   // text
    props->setProperty(6, 0);    // font
}

//MProgress::MProgress(int16_t min, int16_t max)
//{
//    this->min = min;
//    this->max = max;
//    val = 0;
//    position = QRectF(0, 0, 400, 32);
//    type = "prgs";
//}

void MProgress::setValue(int16_t val)
{
    //if((val >= min) && (val <= max))
    //    this->val = val;
    if((val >= 0) && (val <= 100))
        props->setProperty(4, val);
}

//void  MProgress::setRange(int16_t min, int16_t max)
//{
//    this->min = min;
//    this->max = max;
//}

QRectF MProgress::boundingRect() const
{
    //return position;
    return QRectF(
        QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
        PC_SCALE * props->getProperty(1)->data.toInt()),
        QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
        PC_SCALE * props->getProperty(3)->data.toInt()));
}

void MProgress::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    // Рисуем рамку
    QPen pen;
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawRect(boundingRect().x() + 2, boundingRect().y() + 2, boundingRect().width() - 4, boundingRect().height() - 4);
    // Рисуем заполнение
    painter->setBrush(QBrush(Qt::green));
    pen.setWidth(0);
    pen.setColor(Qt::green);
    painter->setPen(pen);
    float realWidth = ((float)props->getProperty(4)->data.toInt() - 0.0f) / (100.0f - 0.0f) * (boundingRect().width() - 8);
    painter->drawRect(boundingRect().x() + 4, boundingRect().y() + 4, realWidth, boundingRect().height() - 9);
}

// ---------------------------------------- MENU --------------------------------------------
MMenu::MMenu(QRectF position)
{
    //this->position = position;
    //font = 0;
//    items.append(new PropItem("Items", PropList, QStringList()));
//    items.append(new PropItem("StartPos", PropNumber, 0));
//    items.append(new PropItem("CurPos", PropNumber, 0));
//    items.append(new PropItem("Font", PropNumber, 0));
    type = "menu";
    props = new MItemProperty(GuiMenu);
    props->setProperty(0, position.topLeft().x());
    props->setProperty(1, position.topLeft().y());
    props->setProperty(2, position.width());
    props->setProperty(3, position.height());
    //props->setProperty(5, -1);  // StartPos
    //props->setProperty(6, -1);  // CurPos
    //props->setProperty(7, 0);   // Font
}

QRectF MMenu::boundingRect() const
{
    return QRectF(
        QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
        PC_SCALE * props->getProperty(1)->data.toInt()),
        QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
        PC_SCALE * props->getProperty(3)->data.toInt()));
}

void MMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Рисуем синий прямоугольник
    QPen pen(Qt::DashLine);
    pen.setColor(selected ? Qt::red : Qt::blue);
    painter->setPen(pen);
    painter->drawRect(boundingRect());

    switch(props->getProperty(7)->data.toInt())
    {
        case 0:
            painter->setFont(QFont("Courier New", 30));
        break;
    }

    QStringList temp = props->getProperty(4)->data.toStringList();
    painter->setPen(Qt::black);
    for(uint16_t i=0; i<temp.length(); i++)
    {
        painter->drawStaticText(boundingRect().topLeft() + QPoint(24, 32*i-8), QStaticText(temp.at(i)));
    }
}

//void MMenu::addMenuItem(QString str)
//{

//}
