#ifndef MITEMTYPEDELEGATE_H
#define MITEMTYPEDELEGATE_H

#include <QStyledItemDelegate>

class MItemTypeDelegate : public QStyledItemDelegate
{
public:
	MItemTypeDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) { }

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MITEMTYPEDELEGATE_H
