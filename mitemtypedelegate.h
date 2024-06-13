#ifndef MITEMTYPEDELEGATE_H
#define MITEMTYPEDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief MItemTypeDelegate - класс делегата для отрисовки типа элемента в QListView
 */
class MItemTypeDelegate : public QStyledItemDelegate
{
public:
	MItemTypeDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) { }

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MITEMTYPEDELEGATE_H
