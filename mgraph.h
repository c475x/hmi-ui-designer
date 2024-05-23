#ifndef MGRAPH_H
#define MGRAPH_H

#include <cstdint>

#include <QObject>
#include <QGraphicsItem>
#include <QImage>
#include <QVector>
#include <QString>

#include "mitempropertymodel.h"

/**
 * @brief MBase - базовый класс графического элемента
 */
class MBase : public QGraphicsItem
{
public:
    MBase() {selected = false;}
    ~MBase() {if (props) delete props;}
    void setSelected(bool val) {selected = val;}
    const char *type;
    MItemProperty *props = NULL;

protected:
    bool selected;
};

/**
 * @brief MIconSet - класс элемента "Иконка"
 */
class MIconSet : public MBase
{
public:
    MIconSet() : MIconSet(QPointF(0, 0), QSizeF(0, 0)) { }
    MIconSet(QPointF origin, QSizeF size);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void addIcon(QImage icon);
    void clearIcons();
    void setCurrent(int16_t pos);

protected:
    QVector<QImage> iconset;
};

/**
 * @brief MLabel - класс элемента "Надпись"
 */
class MLabel : public MBase
{
public:
    MLabel() : MLabel("", QRectF(0, 0, 0, 0)) { }
    MLabel(QString label, QRectF position, uint8_t font = 0);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class MProgress : public MBase
{
public:
    MProgress() : MProgress("", QRectF(0, 0, 0, 0)) { }
    MProgress(QString label, QRectF position);
    MProgress(QString label, QRectF position, int16_t min, int16_t max);
    MProgress(int16_t min, int16_t max);
    void setValue(int16_t val);
    void setRange(int16_t min, int16_t max);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    int16_t min;
    int16_t max;
    int16_t val;
};

/**
 * @brief MMenu - класс элемента "Меню"
 */
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
