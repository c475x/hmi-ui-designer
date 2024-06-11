#include <QPainter>
#include <QFontMetrics>

#include "mitemtypedelegate.h"

void MItemTypeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

	// Тип графического элемента
	QString type = index.data(Qt::BackgroundRole).toString();

	// Ширина имени элемента
	QFontMetrics metrics(painter->font());
	int nameWidth = metrics.horizontalAdvance(index.data(Qt::DisplayRole).toString());

	// Рисуем текст правее имени элемента
	painter->save();
	painter->setPen(Qt::gray);
	QFont font = painter->font();
	font.setItalic(true);
	painter->setFont(font);
	painter->drawText(option.rect.adjusted(nameWidth + 8, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, type);
	painter->restore();
}
