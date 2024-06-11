#include <QPainter>
#include <QStaticText>
#include <QtMath>

#include "mgraph.h"

//
// ----------------------------------------- ICON ---------------------------------------
//

/**
 * @brief MIconSet::MIconSet - создает объект класса MIconSet
 * @param origin - позиция элемента
 * @param size - размер элемента
 */
MIconSet::MIconSet(QPointF origin, QSizeF size)
{
	name = "icon";
	props = new MItemProperty(GuiIconSet);

	props->setProperty(PROP_X, origin.x());
	props->setProperty(PROP_Y, origin.y());
	props->setProperty(PROP_WIDTH, size.width());
	props->setProperty(PROP_HEIGHT, size.height());
}

/**
 * @brief MIconSet::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MIconSet::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(PROP_X)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_Y)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(PROP_WIDTH)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_HEIGHT)->data.toInt())
				);
}

/**
 * @brief MIconSet::paint - функция отрисовки графического элемента
 * @param painter - указатель на объект пеинтера
 * @param option - указатель на объект опций элемента
 * @param widget - указатель на виджет
 */
void MIconSet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	// Цвет обводки по умолчанию синий, при выделении - красный
	// Цвет фона по умолчанию белый, при выделении - зеленый
	QPen pen(Qt::DashLine);
	QBrush brush(selected ? Qt::green : Qt::white);
	pen.setColor(selected ? Qt::red : Qt::blue);
	painter->setPen(pen);
	painter->setBrush(brush);

	// Рисуем границу элемента
	painter->drawRect(boundingRect());

	int16_t current = props->getProperty(PROP_CURINDEX)->data.toInt();
	if ((current >= 0) && (current < iconset.size()))
	{
		// Рисование выбранной иконки внутри прямоугольника элемента
		painter->setPen(Qt::SolidLine);
		painter->setPen(Qt::black);
		painter->drawImage(boundingRect(), iconset.at(current));
	}
	else
	{
		// Рисуем синий крест, если индекс иконки некорректен
		painter->drawLine(boundingRect().topLeft(), boundingRect().bottomRight());
		painter->drawLine(boundingRect().topRight(), boundingRect().bottomLeft());
	}
}

/**
 * @brief MIconSet::addIcon - функция добавляет иконку в сет
 * @param icon - добавляемая иконка
 */
void MIconSet::addIcon(QImage icon)
{
	iconset.append(icon);
}

/**
 * @brief MIconSet::clearIcons - функция очищает список изображений иконки
 */
void MIconSet::clearIcons()
{
	iconset.clear();
}

/**
 * @brief MIconSet::setCurrent - функция задает индекс отображаемой иконки
 * @param pos - индекс отображаемой иконки
 */
void MIconSet::setCurrent(int16_t pos)
{
	if (pos < iconset.size())
	{
		props->setProperty(PROP_CURINDEX, pos);
	}
}

//
// -------------------------------------------- LABEL ------------------------------------------------
//

/**
 * @brief MLabel::MLabel - создает объект класса MLabel
 * @param label - текст элемента
 * @param position - позиция элемента
 * @param font - шрифт
 */
MLabel::MLabel(QString label, QRectF position, uint8_t font)
{
	name = "labl";
	props = new MItemProperty(GuiLabel);

	props->setProperty(PROP_X, position.topLeft().x());
	props->setProperty(PROP_Y, position.topLeft().y());
	props->setProperty(PROP_WIDTH, position.width());
	props->setProperty(PROP_HEIGHT, position.height());
	props->setProperty(PROP_TEXT, label);
	props->setProperty(PROP_FONT, font);
}

/**
 * @brief MLabel::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MLabel::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(PROP_X)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_Y)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(PROP_WIDTH)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_HEIGHT)->data.toInt())
				);
}

/**
 * @brief MLabel::paint - функция отрисовки графического элемента
 * @param painter - указатель на объект пеинтера
 * @param option - указатель на объект опций элемента
 * @param widget - указатель на виджет
 */
void MLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	// Цвет обводки по умолчанию синий
	// Цвет фона по умолчанию белый, при выделении - зеленый
	QPen pen(Qt::DashLine);
	QBrush brush(selected ? Qt::green : Qt::white);
	pen.setColor(Qt::blue);
	painter->setPen(pen);
	painter->setBrush(brush);

	// Рисуем границу элемента
	painter->drawRect(boundingRect());

	switch (props->getProperty(PROP_FONT)->data.toInt())
	{
		case 0:
			painter->setFont(QFont("Courier New", 30));
		break;
	}

	// Рисуем текст, цвет текста черный
	painter->setPen(Qt::black);
	painter->drawStaticText(boundingRect().topLeft(), QStaticText(props->getProperty(PROP_TEXT)->data.toString()));
}

//
// ---------------------------------------- PROGRESS ---------------------------------------------
//

/**
 * @brief MProgress::MProgress - создает объект класса MProgress
 * @param label - текст элемента
 * @param position - позиция элемента
 */
MProgress::MProgress(QString label, QRectF position)
{
	name = "prgs";
	props = new MItemProperty(GuiProgress);

	props->setProperty(PROP_X, position.topLeft().x());
	props->setProperty(PROP_Y, position.topLeft().y());
	props->setProperty(PROP_WIDTH, position.width());
	props->setProperty(PROP_HEIGHT, position.height());
	props->setProperty(PROP_VALUE, 0);
	props->setProperty(PROP_MINVAL, 0);
	props->setProperty(PROP_MAXVAL, 100);
	props->setProperty(PROP_TEXT, label);
	// props->setProperty(PROP_FONT, 0);
}

/**
 * @brief MProgress::MProgress - создает объект класса MProgress
 * @param label - текст элемента
 * @param position - позиция элемента
 * @param min - минимальное значение
 * @param max - максимальное значение
 */
MProgress::MProgress(QString label, QRectF position, int16_t min, int16_t max)
{
	name = "prgs";
	props = new MItemProperty(GuiProgress);

	props->setProperty(PROP_X, position.topLeft().x());
	props->setProperty(PROP_Y, position.topLeft().y());
	props->setProperty(PROP_WIDTH, position.width());
	props->setProperty(PROP_HEIGHT, position.height());
	props->setProperty(PROP_VALUE, min);
	props->setProperty(PROP_MINVAL, min);
	props->setProperty(PROP_MAXVAL, max);
	props->setProperty(PROP_TEXT, label);
	props->setProperty(PROP_FONT, 0);
	props->setProperty(PROP_ISVISIBLE, true);
}
/**
 * @brief MProgress::setValue - задает значение прогрессбара
 * @param val - новое значение прогрессбара
 */
void MProgress::setValue(int16_t val)
{
	if ((val >= min) && (val <= max))
	{
		props->setProperty(PROP_VALUE, val);
	}
}

//void MProgress::setRange(int16_t min, int16_t max)
//{
//	this->min = min;
//	this->max = max;
//}

/**
 * @brief MProgress::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MProgress::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(PROP_X)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_Y)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(PROP_WIDTH)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_HEIGHT)->data.toInt())
				);
}

/**
 * @brief MMenu::paint - функция отрисовки графического элемента
 * @param painter - указатель на объект пеинтера
 * @param option - указатель на объект опций элемента
 * @param widget - указатель на виджет
 */
void MProgress::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	// Рисуем рамку
	QPen pen;
	pen.setWidth(4);
	painter->setPen(pen);
	painter->drawRect(boundingRect().x() + 2, boundingRect().y() + 2, boundingRect().width() - 5, boundingRect().height() - 4);

	// Рисуем заполнение
	painter->setBrush(QBrush(Qt::black));
	pen.setWidth(0);
	pen.setColor(Qt::black);
	painter->setPen(pen);

	float realWidth = ((float)props->getProperty(PROP_VALUE)->data.toInt() - (float)props->getProperty(PROP_MINVAL)->data.toInt()) / ((float)props->getProperty(PROP_MAXVAL)->data.toInt() - (float)props->getProperty(PROP_MINVAL)->data.toInt()) * (boundingRect().width() - 9);
	painter->drawRect(boundingRect().x() + 4, boundingRect().y() + 4, realWidth, boundingRect().height() - 9);
}

//
// ---------------------------------------- MENU --------------------------------------------
//

/**
 * @brief MMenu::MMenu - создает объект класса MProgress
 * @param val - новое значение прогрессбара
 */
MMenu::MMenu(QRectF position)
{
	name = "menu";
	props = new MItemProperty(GuiMenu);

	props->setProperty(PROP_X, position.topLeft().x());
	props->setProperty(PROP_Y, position.topLeft().y());
	props->setProperty(PROP_WIDTH, position.width());
	props->setProperty(PROP_HEIGHT, position.height());
	props->setProperty(PROP_STARTPOS, 0);
	props->setProperty(PROP_CURPOS, -1);
	//props->setProperty(PROP_FONT, 0);
}

/**
 * @brief MMenu::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MMenu::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(PROP_X)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_Y)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(PROP_WIDTH)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_HEIGHT)->data.toInt())
				);
}

/**
 * @brief MMenu::paint - функция отрисовки графического элемента
 * @param painter - указатель на объект пеинтера
 * @param option - указатель на объект опций элемента
 * @param widget - указатель на виджет
 */
void MMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	// Цвет обводки по умолчанию синий, при выделении - красный
	QPen pen(Qt::DashLine);
	pen.setColor(selected ? Qt::red : Qt::blue);
	painter->setPen(pen);

	// Рисуем границу элемента
	painter->drawRect(boundingRect());

	// Установка выбранного шрифта
	switch (props->getProperty(PROP_FONT)->data.toInt())
	{
		case 0:
			painter->setFont(QFont("Courier New", 30));
		break;
	}

	// Получаем список текстовых строк меню
	QStringList temp = props->getProperty(PROP_ITEMS)->data.toStringList();

	// По умолчанию шрифт строки черный
	painter->setPen(Qt::black);
	painter->setBrush(QBrush(Qt::black));

	// Если текущая позиция неактивна (меньше нуля), отобразить как -1
	if (props->getProperty(PROP_CURPOS)->data.toInt() < 0)
	{
		props->setProperty(PROP_CURPOS, -1);
	}

	// Если текущая позиция больше размера списка
	if (props->getProperty(PROP_CURPOS)->data.toInt() >= temp.length())
	{
		props->setProperty(PROP_CURPOS, temp.length() - 1);
	}

	// Если текущая позиция меньше начальной позиции
	if (props->getProperty(PROP_CURPOS)->data.toInt() < props->getProperty(PROP_STARTPOS)->data.toInt())
	{
		if (props->getProperty(PROP_STARTPOS)->data.toInt() > 0)
		{
			props->setProperty(PROP_STARTPOS, props->getProperty(PROP_CURPOS)->data.toInt());
		}
	}

	// Если текущая позиция больше начальной позиции + размера блока
	if (props->getProperty(PROP_CURPOS)->data.toInt() > props->getProperty(PROP_STARTPOS)->data.toInt() + qFloor(props->getProperty(PROP_HEIGHT)->data.toInt() * PC_SCALE / 35) - 1)
	{
		if (props->getProperty(PROP_STARTPOS)->data.toInt() + 1 < temp.length() - 1)
		{
			props->setProperty(PROP_STARTPOS, qBound(0, props->getProperty(PROP_CURPOS)->data.toInt() - qFloor(props->getProperty(PROP_HEIGHT)->data.toInt() * PC_SCALE / 35) + 1, temp.length() - 1));
		}
	}

	// Начальная позиция
	int startPos = props->getProperty(PROP_STARTPOS)->data.toInt();

	// Перебираем все строки из списка
	for (uint16_t i = startPos; i < temp.length(); i++)
	{
		// Прерывание цикла отведено отдельно, чтобы изменение текущего элемента успело прорисоваться
		if (35 * (i + 1 - startPos) > props->getProperty(PROP_HEIGHT)->data.toInt() * PC_SCALE)
		{
			break;
		}

		// Если один из пунктов меню - с выделением...
		if (i == props->getProperty(PROP_CURPOS)->data.toInt())
		{
			// Рисуем черный прямоугольник выделения
			painter->drawRect(boundingRect().topLeft().x(),
						  boundingRect().topLeft().y() + 35 * (i - startPos),
						  boundingRect().topRight().x() - boundingRect().topLeft().x(),
						  35
						);

			// Белый цвет для текста выделенного элемента
			painter->setPen(Qt::white);
		}

		// Рисуем текст
		painter->drawStaticText(boundingRect().topLeft() + QPoint(24, 35 * (i - startPos)), QStaticText(temp.at(i)));

		// Возвращаем черный цвет для следующей строки
		if (i == props->getProperty(PROP_CURPOS)->data.toInt())
			painter->setPen(Qt::black);
	}
}
