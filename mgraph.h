#ifndef MGRAPH_H
#define MGRAPH_H

#include <stdint.h>
#include <QObject>
#include <QGraphicsItem>
#include <QImage>
#include <QVector>
#include <QString>
#include "mitempropertymodel.h"

class MBase : public QGraphicsItem
{
public:
    MBase() {selected = false;}
    ~MBase() {if(props) delete props;}
    void setSelected(bool val) {selected = val;}
    const char *type;
    MItemProperty *props = NULL;

protected:
    //QRectF position;
    bool selected;
};

class MIconSet : public MBase
{
public:
    MIconSet() : MIconSet(QPointF(0, 0), QSizeF(0, 0)) {}
    MIconSet(QPointF origin, QSizeF size);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void addIcon(QImage icon);
    void clearIcons();
    void setCurrent(int16_t pos);

protected:
    QVector<QImage> set;
};

class MLabel : public MBase
{
public:
    MLabel() : MLabel(QRectF(0, 0, 0, 0), 0) {}
    MLabel(QRectF position, QString str, uint8_t font = 0);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

};

class MProgress : public MBase
{
public:
    MProgress() : MProgress(QRectF(0, 0, 0, 0)) {}
    MProgress(QRectF position);
    //MProgress(int16_t min, int16_t max);
    void setValue(int16_t val);
    //void  setRange(int16_t min, int16_t max);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    //QRectF position;
    //int16_t min;
    //int16_t max;
    //int16_t val;
};

class MMenu : public MBase
{
public:
    //MMenu(QRectF position) {this->position = position; font = 0; type = "menu";}
    MMenu(QRectF position);// {this->position = position; font = 0; type = "menu";}
    //void addMenuItem(QString str) {items.append(str);}
    //void addMenuItem(QString str);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    //QVector<QString> items;
    //uint8_t font;
};

#endif // MGRAPH_H
