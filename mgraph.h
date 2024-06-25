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
	MBase() { selected = false; }
	~MBase() { if (props) delete props; }
	void setSelected(bool val) { selected = val; }
	QString name;
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
	typedef enum
	{
		PROP_X,
		PROP_Y,
		PROP_WIDTH,
		PROP_HEIGHT,
		PROP_ICONS,
		PROP_CURINDEX
	} ItemProperty;

	MIconSet() : MIconSet("icon", QPointF(0, 0), QSizeF(0, 0)) { }
	MIconSet(QString name, QPointF origin, QSizeF size);
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
	typedef enum
	{
		PROP_X,
		PROP_Y,
		PROP_WIDTH,
		PROP_HEIGHT,
		PROP_TEXT,
		PROP_FONT
	} ItemProperty;

	MLabel() : MLabel("label", "", QRectF(0, 0, 0, 0)) { }
	MLabel(QString name, QString label, QRectF position, uint8_t font = 0);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

/**
 * @brief MProgress - класс элемента "Прогресс"
 */
class MProgress : public MBase
{
public:
	typedef enum
	{
		PROP_X,
		PROP_Y,
		PROP_WIDTH,
		PROP_HEIGHT,
		PROP_VALUE,
		PROP_MINVAL,
		PROP_MAXVAL,
		PROP_TEXT,
		PROP_ISVISIBLE,
		PROP_FONT,
	} ItemProperty;

	MProgress() : MProgress("progress", "", QRectF(0, 0, 0, 0)) { }
	MProgress(QString name, QString label, QRectF position);
	MProgress(QString name, QString label, QRectF position, int16_t min, int16_t max);
	MProgress(QString name, int16_t min, int16_t max);
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
	typedef enum
	{
		PROP_X,
		PROP_Y,
		PROP_WIDTH,
		PROP_HEIGHT,
		PROP_ITEMS,
		PROP_STARTPOS,
		PROP_CURPOS,
		PROP_FONT
	} ItemProperty;

	MMenu(QString name, QRectF position);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;

protected:
	//uint8_t font;
};

/**
 * @brief MCombo - класс элемента "Комбо"
 */
class MCombo : public MBase
{
public:
	typedef enum
	{
		PROP_X,
		PROP_Y,
		PROP_WIDTH,
		PROP_HEIGHT,
		PROP_LABEL,
		PROP_ITEMS,
		PROP_CURPOS,
		PROP_IS_SELECTED,
		PROP_FONT
	} ItemProperty;

	MCombo(QString name, QString label, QRectF position);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;

protected:
	//uint8_t font;
};

#endif // MGRAPH_H
