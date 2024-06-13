#include <QPainter>
#include <QFontMetrics>

#include "mitemtypedelegate.h"

/**
 * @brief MItemTypeDelegate::paint - функция отрисовки типа элемента
 * @param painter - объект для рисования
 * @param option - параметры стиля
 * @param index - индекс элемента
 */
void MItemTypeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	// Отрисовка родительского элемента
	QStyledItemDelegate::paint(painter, option, index);

	// Тип графического элемента
	QString itemTypeStr = index.data(Qt::BackgroundRole).toString();

	// Ширина имени элемента
	QFontMetrics metrics(painter->font());
	int nameWidth = metrics.horizontalAdvance(index.data(Qt::DisplayRole).toString());

	// Рисуем текст правее имени элемента
	painter->save();
	painter->setPen(Qt::gray);
	QFont font = painter->font();
	font.setItalic(true);
	painter->setFont(font);
	painter->drawText(option.rect.adjusted(nameWidth + 8, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, itemTypeStr);
	painter->restore();
}
