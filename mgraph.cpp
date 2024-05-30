#include <QPainter>
#include <QStaticText>

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
	type = "icon";
	props = new MItemProperty(GuiIconSet);

	props->setProperty(0, origin.x());
	props->setProperty(1, origin.y());
	props->setProperty(2, size.width());
	props->setProperty(3, size.height());
}

/**
 * @brief MIconSet::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MIconSet::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
		PC_SCALE * props->getProperty(1)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
		PC_SCALE * props->getProperty(3)->data.toInt())
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

	int16_t current = props->getProperty(5)->data.toInt();
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
		props->setProperty(5, pos);
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
	type = "labl";
	props = new MItemProperty(GuiLabel);

	props->setProperty(0, position.topLeft().x());
	props->setProperty(1, position.topLeft().y());
	props->setProperty(2, position.width());
	props->setProperty(3, position.height());
	props->setProperty(4, label);
	props->setProperty(5, font);
}

/**
 * @brief MLabel::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MLabel::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
		PC_SCALE * props->getProperty(1)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
		PC_SCALE * props->getProperty(3)->data.toInt())
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

	switch (props->getProperty(5)->data.toInt())
	{
		case 0:
			painter->setFont(QFont("Courier New", 30));
		break;
	}

    // Рисуем текст, цвет текста черный
	painter->setPen(Qt::black);
	painter->drawStaticText(boundingRect().topLeft(), QStaticText(props->getProperty(4)->data.toString()));
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
	type = "prgs";
	props = new MItemProperty(GuiProgress);

	props->setProperty(0, position.topLeft().x());
	props->setProperty(1, position.topLeft().y());
	props->setProperty(2, position.width());
	props->setProperty(3, position.height());
	props->setProperty(4, 0);	   // value
	props->setProperty(5, 0);	   // min
	props->setProperty(6, 100);	 // max
	props->setProperty(7, label);   // text
	// props->setProperty(8, 0);	   // font
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
	type = "prgs";
	props = new MItemProperty(GuiProgress);

	props->setProperty(0, position.topLeft().x());
	props->setProperty(1, position.topLeft().y());
	props->setProperty(2, position.width());
	props->setProperty(3, position.height());
    props->setProperty(4, min);     // value
    props->setProperty(5, min);     // min
    props->setProperty(6, max);     // max
	props->setProperty(7, label);   // text
    props->setProperty(8, 0);       // font
    props->setProperty(9, true);    // isvisible

/**
 * @brief MProgress::setValue - задает значение прогрессбара
 * @param val - новое значение прогрессбара
 */
void MProgress::setValue(int16_t val)
{
	if ((val >= min) && (val <= max))
		props->setProperty(4, val);
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
		QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
		PC_SCALE * props->getProperty(1)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
		PC_SCALE * props->getProperty(3)->data.toInt())
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

    if ()

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

	float realWidth = ((float)props->getProperty(4)->data.toInt() - (float)props->getProperty(5)->data.toInt()) / ((float)props->getProperty(6)->data.toInt() - (float)props->getProperty(5)->data.toInt()) * (boundingRect().width() - 9);
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
	type = "menu";
	props = new MItemProperty(GuiMenu);

	props->setProperty(0, position.topLeft().x());
	props->setProperty(1, position.topLeft().y());
	props->setProperty(2, position.width());
	props->setProperty(3, position.height());
	props->setProperty(5, -1);  // StartPos
	props->setProperty(6, -1);  // CurPos
	//props->setProperty(7, 0);   // Font
}

/**
 * @brief MMenu::boundingRect - функция получения границ элемента
 * @return - границы элемента
 */
QRectF MMenu::boundingRect() const
{
	return QRectF(
		QPointF(PC_SCALE * props->getProperty(0)->data.toInt(),
		PC_SCALE * props->getProperty(1)->data.toInt()),
		QSizeF(PC_SCALE * props->getProperty(2)->data.toInt(),
		PC_SCALE * props->getProperty(3)->data.toInt())
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
    // Цвет обводки по умолчанию синий, при выделении - красный
	QPen pen(Qt::DashLine);
	pen.setColor(selected ? Qt::red : Qt::blue);
	painter->setPen(pen);

    // Рисуем границу элемента
    painter->drawRect(boundingRect());

    // Установка выбранного шрифта
	switch (props->getProperty(7)->data.toInt())
	{
		case 0:
			painter->setFont(QFont("Courier New", 30));
		break;
	}

    // Получаем список текстовых строк меню
	QStringList temp = props->getProperty(4)->data.toStringList();

    // По умолчанию шрифт строки черный
	painter->setPen(Qt::black);
	painter->setBrush(QBrush(Qt::black));

    // Заданная пользователем начальная позиция, если указано некорректное значение - приравняется к 0
    int startPos = (props->getProperty(5)->data.toInt() >= 0) &&
                (props->getProperty(5)->data.toInt() < temp.length()) ? props->getProperty(5)->data.toInt() : 0;

    // Если текущая позиция меньше начальной позиции и не равна -1, приравняем ее к начальной позиции
    if (props->getProperty(6)->data.toInt() < props->getProperty(5)->data.toInt() && props->getProperty(6)->data.toInt() != -1)
    {
        props->setProperty(6, startPos);
    }

    // Перебираем все строки из списка
    for (uint16_t i = startPos; i < temp.length(); i++)
	{
        // Прервать цикл, если строка вылазит за пределы элемента
        if (35 * (i - startPos) > props->getProperty(3)->data.toInt() * PC_SCALE)
        {
            break;
        }

        // Если один из пунктов меню - с выделением...
		if (i == props->getProperty(6)->data.toInt())
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
		if (i == props->getProperty(6)->data.toInt())
			painter->setPen(Qt::black);
	}
}
