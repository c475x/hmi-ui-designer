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
MIconSet::MIconSet(QString name, QPointF origin, QSizeF size)
{
	this->name = name;
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
MLabel::MLabel(QString name, QString label, QRectF position, uint8_t font)
{
	this->name = name;
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

	// Индекс шрифта не может быть отрицательным
	if (props->getProperty(PROP_FONT)->data.toInt() < 0)
	{
		props->setProperty(PROP_FONT, 0);
	}

	switch (props->getProperty(PROP_FONT)->data.toInt())
	{
		default:
			painter->setFont(QFont("Courier New", 30));
		break;
		case 1:
			painter->setFont(QFont("Courier New", 60));
		break;
	}

	// Рисуем текст, цвет текста черный
	painter->setPen(Qt::black);
	painter->drawText(boundingRect(), props->getProperty(PROP_TEXT)->data.toString());
}

//
// ---------------------------------------- PROGRESS ---------------------------------------------
//

/**
 * @brief MProgress::MProgress - создает объект класса MProgress
 * @param label - текст элемента
 * @param position - позиция элемента
 */
MProgress::MProgress(QString name, QString label, QRectF position)
{
	this->name = name;
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
MProgress::MProgress(QString name, QString label, QRectF position, int16_t min, int16_t max)
{
	this->name = name;
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

	// Цвет обводки по умолчанию синий, при выделении - красный
	QPen boundingBoxPen(Qt::DashLine);
	boundingBoxPen.setColor(selected ? Qt::red : Qt::blue);
	painter->setPen(boundingBoxPen);

	// Рисуем границу элемента
	painter->drawRect(boundingRect());

	// Установка выбранного шрифта
	switch (props->getProperty(PROP_FONT)->data.toInt())
	{
		case 0:
			painter->setFont(QFont("Courier New", 30));
		break;
	}

	// По умолчанию цвет черный
	QPen pen;
	pen.setColor(Qt::black);
	painter->setPen(pen);

	// Рисуем текст
	QRectF textRect = QRectF(boundingRect().topLeft() + QPointF(2, 2), boundingRect().bottomRight() - QPointF(2, 2));
	painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, props->getProperty(PROP_TEXT)->data.toString());

	// Расчет границ прогрессбара
	QFontMetrics metrics(painter->font());
	int textWidth = metrics.horizontalAdvance(props->getProperty(PROP_TEXT)->data.toString());
	QRectF progressRect = QRectF(boundingRect().topLeft() + QPointF(2 + textWidth, 2), boundingRect().bottomRight() - QPointF(2, 2));

	// Рисуем рамку
	pen.setWidth(2);
	painter->setPen(pen);
	painter->drawRect(progressRect.x() + 2, progressRect.y() + 2, progressRect.width() - 4, progressRect.height() - 4);

	// Рисуем заполнение
	painter->setBrush(QBrush(Qt::black));
	pen.setWidth(0);
	pen.setColor(Qt::black);
	painter->setPen(pen);

	float realWidth = ((float)props->getProperty(PROP_VALUE)->data.toInt() - (float)props->getProperty(PROP_MINVAL)->data.toInt()) / ((float)props->getProperty(PROP_MAXVAL)->data.toInt() - (float)props->getProperty(PROP_MINVAL)->data.toInt()) * (progressRect.width() - 6);
	painter->drawRect(progressRect.x() + 2, progressRect.y() + 3, realWidth, progressRect.height() - 7);
}

//
// ---------------------------------------- MENU --------------------------------------------
//

/**
 * @brief MMenu::MMenu - создает объект класса MMenu
 * @param val - новое значение прогрессбара
 */
MMenu::MMenu(QString name, QRectF position)
{
	this->name = name;
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

	// Высота строки
	int rowHeight = 35;

	// Перебираем все строки из списка
	for (uint16_t i = startPos; i < temp.length(); i++)
	{
		// Прерывание цикла отведено отдельно, чтобы изменение текущего элемента успело прорисоваться
		if (rowHeight * (i + 1 - startPos) > props->getProperty(PROP_HEIGHT)->data.toInt() * PC_SCALE)
		{
			break;
		}

		// Если один из пунктов меню - с выделением...
		if (i == props->getProperty(PROP_CURPOS)->data.toInt())
		{
			// Рисуем черный прямоугольник выделения
			painter->drawRect(boundingRect().topLeft().x(),
						  boundingRect().topLeft().y() + rowHeight * (i - startPos),
						  boundingRect().topRight().x() - boundingRect().topLeft().x(),
						  rowHeight
						);

			// Белый цвет для текста выделенного элемента
			painter->setPen(Qt::white);
		}

		// Рисуем текст
		painter->drawStaticText(boundingRect().topLeft() + QPoint(24, rowHeight * (i - startPos)), QStaticText(temp.at(i)));

		// Возвращаем черный цвет для следующей строки
		if (i == props->getProperty(PROP_CURPOS)->data.toInt())
			painter->setPen(Qt::black);
	}
}

//
// ---------------------------------------- COMBO --------------------------------------------
//

/**
 * @brief MCombo::MCombo - создает объект класса MProgress
 * @param val - новое значение прогрессбара
 */
MCombo::MCombo(QString name, QString label, QRectF position)
{
	this->name = name;
	props = new MItemProperty(GuiCombo);

	props->setProperty(PROP_X, position.topLeft().x());
	props->setProperty(PROP_Y, position.topLeft().y());
	props->setProperty(PROP_WIDTH, position.width());
	props->setProperty(PROP_HEIGHT, position.height());
	props->setProperty(PROP_LABEL, label);
	props->setProperty(PROP_CURPOS, 0);
	props->setProperty(PROP_IS_SELECTED, 0);
	props->setProperty(PROP_FONT, 0);
}

/**
 * @brief MCombo::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MCombo::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(PROP_X)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_Y)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(PROP_WIDTH)->data.toInt(),
		PC_SCALE * props->getProperty(PROP_HEIGHT)->data.toInt())
				);
}

/**
 * @brief MCombo::paint - функция отрисовки графического элемента
 * @param painter - указатель на объект пеинтера
 * @param option - указатель на объект опций элемента
 * @param widget - указатель на виджет
 */
void MCombo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

	// Получаем список текстовых строк и их возможных значений
	QStringList temp = props->getProperty(PROP_ITEMS)->data.toStringList();

	// По умолчанию шрифт строки черный
	painter->setPen(Qt::black);
	painter->setBrush(QBrush(Qt::black));

	// Если текущая позиция неактивна (меньше нуля), отобразить как -1
	if (props->getProperty(PROP_CURPOS)->data.toInt() < 0 && temp.length() > 0)
	{
		props->setProperty(PROP_CURPOS, 0);
	}
	else if (props->getProperty(PROP_CURPOS)->data.toInt() < 0)
	{
		props->setProperty(PROP_CURPOS, -1);
	}

	// Если текущая позиция больше размера списка
	if (props->getProperty(PROP_CURPOS)->data.toInt() >= temp.length())
	{
		props->setProperty(PROP_CURPOS, temp.length() - 1);
	}

	// Если один элемент является активным...
	if (props->getProperty(PROP_IS_SELECTED)->data.toInt() == 1)
	{
		// Рисуем черный прямоугольник выделения
		painter->drawRect(boundingRect().topLeft().x(),
					  boundingRect().topLeft().y(),
					  boundingRect().topRight().x() - boundingRect().topLeft().x(),
					  boundingRect().height()
					);

		// Белый цвет для текста выделенного элемента
		painter->setPen(Qt::white);
	}

	// Индекс выбранного элемента
	int curPos = props->getProperty(PROP_CURPOS)->data.toInt();

	// Ширина значения свойства (для предотвращения накладывания названия на значение)
	QFontMetrics metrics(painter->font());
	int valWidth = (curPos != -1) ? metrics.horizontalAdvance(temp.at(curPos)) : 0;

	// Границы названия свойства
	QRectF propNameRect = QRectF(boundingRect().topLeft() + QPoint(24, 0),
								 QSizeF(boundingRect().width() - 48 - valWidth - 8, metrics.height()));

	// Рисуем название свойства
	painter->drawText(propNameRect, Qt::AlignLeft | Qt::AlignVCenter, props->getProperty(PROP_LABEL)->data.toString());

	// Рисуем значение свойства
	if (curPos != -1)
	{
		// Границы значения свойства (с выравниванием по правому краю)
		QRectF valRect = QRectF(boundingRect().topLeft() + QPoint(24, 0),
								QSizeF(boundingRect().width() - 48, metrics.height()));

		painter->drawText(valRect, Qt::AlignRight | Qt::AlignVCenter, temp.at(curPos));
	}

	// Возвращаем черный цвет для следующей строки
	if (props->getProperty(PROP_IS_SELECTED)->data.toInt() == 1)
	{
		painter->setPen(Qt::black);
	}
}
